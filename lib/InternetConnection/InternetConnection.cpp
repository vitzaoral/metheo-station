#include "InternetConnection.h"
#include "../../src/settings.cpp"
#include <BlynkSimpleEsp8266.h>

WiFiClient client;
Settings settings;

const char *ssid = settings.ssid;
const char *ssid2 = settings.ssid2;
const char *password = settings.password;
const char *blynkAuth = settings.blynkAuth;

// number of attempts to connecting WIFI,API etc.
const int timeout = 20;

// Initialize WiFi connection. Return true if connection is successful.
bool InternetConnection::initialize(void)
{
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
    {
        Serial.println("no networks found");
        return false;
    }
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        int strength = 1000;
        String usedSSID = "";

        // connect to the strongest known WiFi
        for (int i = 0; i < n; ++i)
        {
            int rssi = WiFi.RSSI(i);
            String actualSsid = WiFi.SSID(i);
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(actualSsid);
            Serial.print(" (");
            Serial.print(rssi);
            Serial.println(" dBi)");

            if (abs(rssi) < strength && (actualSsid == ssid || actualSsid == ssid2))
            {
                usedSSID = actualSsid;
                strength = abs(rssi);
            }
        }

        WiFi.begin(usedSSID.c_str(), password);

        Serial.print("WiFi connecting to: ");
        Serial.println(usedSSID);

        int i = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
            if (i == timeout)
            {
                Serial.println("Timeout on WiFi connection");
                return false;
            }
            i++;
        }
        Serial.println("");
        Serial.print("WiFi connected to:");
        Serial.println(WiFi.SSID());
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Wifi signal stregth: ");
        Serial.println(WiFi.RSSI());

        return true;
    }
}

// Initialize connection to Blynk. Return true if connection is successful.
bool InternetConnection::initializeBlynk(void)
{
    // TODO: tohle vola wifi.begin, neda se to nejak obejit?
    Blynk.config(blynkAuth);

    Serial.println("WiFi connecting to Blynk - 1");
    Blynk.connect();

    if (!Blynk.connected())
    {
        Serial.println("WiFi connecting to Blynk - 2");
        Blynk.connect();
    }

    if (!Blynk.connected())
    {
        Serial.println("WiFi connecting to Blynk - 3");
        Blynk.connect();
    }

    Serial.println(Blynk.connected() ? "Blynk connected" : "Timeout on Blynk or no internet connection");
    return Blynk.connected();
}

void InternetConnection::sendDataToBlynk(MetheoData metheoData, PowerController powerController, bool validData)
{
    // create data to send to Blynk
    if (Blynk.connected())
    {
        // meteo data
        Blynk.virtualWrite(V1, metheoData.temperature);
        Blynk.virtualWrite(V3, metheoData.humidity);
        Blynk.virtualWrite(V4, metheoData.presure);
        // power data
        Blynk.virtualWrite(V2, powerController.batteryVoltage);
        Blynk.virtualWrite(V5, powerController.busVoltage);
        Blynk.virtualWrite(V6, powerController.shuntVoltage);
        Blynk.virtualWrite(V7, powerController.loadVoltage);
        Blynk.virtualWrite(V8, powerController.current_mA);
        Blynk.virtualWrite(V9, powerController.power_mW);
        // send local IP address, WIFI signal stregth and WIFI name
        int signalStrenght = WiFi.RSSI();
        Blynk.virtualWrite(V11, WiFi.localIP().toString());
        Blynk.virtualWrite(V12, signalStrenght);
        Blynk.virtualWrite(V15, WiFi.SSID());

        setStatusToBlynk(validData, validData ? "Meteo data OK" : "Meteo data are invalid", V10);

        // send data to Blynk webhook for ThingSpeak - meteo data channell
        Blynk.virtualWrite(
            V20,
            metheoData.temperature, metheoData.humidity, metheoData.presure);

        // wait because of Blynk webhook limitation (1 request per 1 sec)
        delay(1100);

        // send data to Blynk webhook for ThingSpeak - technical data channel
        Blynk.virtualWrite(
            V21,
            powerController.batteryVoltage, powerController.busVoltage, powerController.shuntVoltage,
            powerController.loadVoltage, powerController.current_mA, powerController.power_mW, signalStrenght);
    }
    else
    {
        Serial.println("Blynk is not connected");
    }
}

// Static method - send message status to Blynk
void InternetConnection::setStatusToBlynk(bool validData, String message, int virtualPin)
{
    char *color = (char *)"#00FF00";

    if (!validData)
    {
        color = (char *)"#FF0000";
    }

    Blynk.virtualWrite(virtualPin, message);
    Blynk.setProperty(virtualPin, "color", color);
}

void InternetConnection::checkForUpdates()
{
    bool updateSuccessful = false;
    String message = "";

    String fileName = String(settings.firmwareFileName);
    String fwURL = String(settings.firmawareUrlBase);
    fwURL.concat(fileName);
    String fwVersionURL = fwURL;
    fwVersionURL.concat(String(settings.firmwareVersionFileName));

    Serial.println("Checking for firmware updates.");
    Serial.print("Firmware version URL: ");
    Serial.println(fwVersionURL);

    HTTPClient httpClient;
    httpClient.begin(fwVersionURL);
    int httpCode = httpClient.GET();
    if (httpCode == 200)
    {
        String newFwVersion = httpClient.getString();

        Serial.print("Current firmware version: ");
        Serial.println(settings.version);
        Serial.print("Available firmware version: ");
        Serial.println(newFwVersion);

        if (String(settings.version) != newFwVersion)
        {
            Serial.println("Preparing to update");

            String fwImageURL = fwURL;
            fwImageURL.concat(".bin");
            t_httpUpdate_return ret = ESPhttpUpdate.update(fwImageURL);

            switch (ret)
            {
            case HTTP_UPDATE_OK:
                message = "Successfuly updated!";
                updateSuccessful = true;
                Serial.println(message);
                break;
            case HTTP_UPDATE_FAILED:
                Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                message = "Update failed: " + String(ESPhttpUpdate.getLastErrorString());
                break;

            case HTTP_UPDATE_NO_UPDATES:
                message = "No updates";
                Serial.println(message);
                break;
            }
        }
        else
        {
            message = "Already on latest version";
            Serial.println(message);
            updateSuccessful = true;
        }
    }
    else
    {
        message = "Version check failed, http code: " + String(httpCode);
        Serial.println(message);
    }
    httpClient.end();

    Blynk.virtualWrite(V13, settings.version);
    setStatusToBlynk(updateSuccessful, message, V14);
}

void InternetConnection::runBlynk()
{
    Blynk.run();
    Serial.println("Send data to Blynk OK");
}