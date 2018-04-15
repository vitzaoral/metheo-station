#include "InternetConnection.h"
#include "../../src/settings.cpp"
#include <BlynkSimpleEsp8266.h>

WiFiClient client;
Settings settings;

const char *thingSpeakWriteApiKey = settings.thingSpeakWriteApiKey;
const unsigned long thingSpeakChannelId = settings.thingSpeakChannelId;
const char *ssid = settings.ssid;
const char *password = settings.password;
const char *blynkAuth = settings.blynkAuth;

// number of attempts to connecting WIFI,API etc.
const int timeout = 10;

// Initialize WiFi connection and ThingSpeak. Return true if connection is sucessfull.
bool InternetConnection::initializeThingSpeak(void)
{
    if (WiFi.SSID() != ssid)
    {
        WiFi.begin(ssid, password);
        WiFi.persistent(true);
        WiFi.setAutoConnect(true);
        WiFi.setAutoReconnect(true);
    }

    Serial.print("WiFi connecting to: ");
    Serial.println(ssid);
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
    Serial.println("WiFi connected");

    ThingSpeak.begin(client);
    return true;
}

// Initialize WiFi connection and Blynk. Return true if connection is sucessfull.
bool InternetConnection::initializeBlynk(void)
{
    Serial.println("WiFi connecting to Blynk");
    // TODO: tohle vola wifi.begin, neda se to nejak obejit?
    Blynk.config(blynkAuth);

    // timeout 3sec
    Blynk.connect(1000);
    Serial.println(Blynk.connected() ? "Blynk connected" : "Timeout on Blynk or no internet connection");
    return Blynk.connected();
}

void InternetConnection::setMeteoDataToThingSpeakObject(MetheoData metheoData, float batteryVoltage)
{
    // create data to send to ThingSpeak
    ThingSpeak.setField(1, metheoData.temperature);
    ThingSpeak.setField(2, batteryVoltage);
    ThingSpeak.setField(3, metheoData.presure);
    ThingSpeak.setField(4, metheoData.humidity);
}

void InternetConnection::sendDataToBlynk(MetheoData metheoData, float batteryVoltage, bool validData)
{
    // create data to send to Blynk
    if (Blynk.connected())
    {
        Blynk.virtualWrite(V1, metheoData.temperature);
        Blynk.virtualWrite(V2, batteryVoltage);
        Blynk.virtualWrite(V3, metheoData.humidity);
        Blynk.virtualWrite(V4, metheoData.presure);
        setStatusToBlynk(validData);
        Blynk.run();
        Serial.println("Send data to Blynk OK");
    }
    else
    {
        Serial.println("Blynk is not connected");
    }
}

bool InternetConnection::sendDataToThingSpeakApi(void)
{
    // Send data in one API call
    int status = ThingSpeak.writeFields(thingSpeakChannelId, thingSpeakWriteApiKey);
    if (status == OK_SUCCESS)
    {
        Serial.println("Send data to Thingspeak OK");
    }
    else
    {
        Serial.print("Error during sending data to ThingSpeak, status code: ");
        Serial.println(status);
    }
    return status;
}

// Static method - send message status to Blynk
void InternetConnection::setStatusToBlynk(bool validData)
{
    char *status = (char *)"Data are OK";
    char *color = (char *)"#00FF00";

    if (!validData)
    {
        status = (char *)"Data are invalid";
        color = (char *)"#FF0000";
    }

    Blynk.virtualWrite(V5, status);
    Blynk.setProperty(V5, "color", color);
}
