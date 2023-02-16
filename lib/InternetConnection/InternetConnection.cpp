#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLHWEX5tqq"
#define BLYNK_DEVICE_NAME "Meteostanice Grun"
#define BLYNK_FIRMWARE_VERSION "2.0.4"

#include "InternetConnection.h"
#include "../../src/settings.cpp"
#include <BlynkSimpleEsp8266.h>

WiFiClient client;
Settings settings;

const char *ssid = settings.ssid;
const char *password = settings.password;
const char *blynkAuth = settings.blynkAuth;
const char *version = settings.version;

// number of attempts to connecting WIFI,API etc.
const int timeout = 20;

String overTheAirURL = "";

BLYNK_WRITE(InternalPinOTA)
{
    Serial.println("OTA Started");
    overTheAirURL = param.asString();
    Serial.print("overTheAirURL = ");
    Serial.println(overTheAirURL);

    HTTPClient http;
    http.begin(client, overTheAirURL);

    t_httpUpdate_return ret = ESPhttpUpdate.update(client, overTheAirURL);
    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
        Serial.println("[update] Update failed.");
        break;
    case HTTP_UPDATE_NO_UPDATES:
        Serial.println("[update] Update no Update.");
        break;
    case HTTP_UPDATE_OK:
        Serial.println("[update] Update ok."); // may not be called since we reboot the ESP
        break;
    }
}

// Initialize connection to Blynk. Return true if connection is successful.
bool InternetConnection::initializeBlynk(void)
{
    int connAttempts = 0;
    Serial.println("\r\nTry connecting to: " + String(ssid));

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        if (connAttempts > 30)
        {
            Serial.println("Error - couldn't connect to WIFI");
            break;
        }

        connAttempts++;
    }

    Blynk.config(blynkAuth);
    Blynk.connect(2000);

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
        Blynk.virtualWrite(V13, version);

        setStatusToBlynk(validData, validData ? "Meteo data OK" : "Meteo data are invalid", V10);

        // set meteo data for webcamera (php scripts) and Blynk webhook for ThingSpeak - meteo data channell
        Blynk.virtualWrite(
            V20,
            metheoData.temperature, "&", metheoData.humidity, "&", metheoData.presure);

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

void InternetConnection::runBlynk()
{
    Blynk.run();
    Serial.println("Send data to Blynk OK");
}