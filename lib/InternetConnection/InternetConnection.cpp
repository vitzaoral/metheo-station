#include "InternetConnection.h"
#include "../../src/settings.cpp"
#include <BlynkSimpleEsp8266.h>

WiFiClient client;
Settings settings;

const char *thingSpeakWriteApiKey = settings.thingSpeakWriteApiKey;
const unsigned long thingSpeakChannelId = settings.thingSpeakChannelId;
const char *ssid = settings.ssid;
const char *password = settings.password;
const char *server = settings.thingSpeakserver;
const char *blynkAuth = settings.blynkAuth;

// Initialize WiFi connection and ThingSpeak. Return true if connection is sucessfull.
bool InternetConnection::initializeThingSpeak(void)
{
    Serial.print("WiFi connecting to: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    // TODO: resit jen nekolik poctu pripojeni pokud WIFI neni at nejede while donekonecna
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
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
    Blynk.begin(settings.blynkAuth, settings.ssid, settings.password);

    while (Blynk.connect() != 1)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Blynk connected");

    return true;
}

void InternetConnection::setMeteoDataToThingSpeakObject(MetheoData metheoData)
{
    // create data to send to ThingSpeak
    ThingSpeak.setField(1, metheoData.shtTemperature);
    ThingSpeak.setField(2, metheoData.bmpTemperature);
    ThingSpeak.setField(3, metheoData.bmpPresure);
    ThingSpeak.setField(4, metheoData.shtHumidity);
}

void InternetConnection::sendDataToBlynk(MetheoData metheoData)
{
    // create data to send to Blynk. For some reason need to call connect.
    Serial.println(Blynk.connect());

    Blynk.run();
    Blynk.virtualWrite(1, metheoData.shtTemperature);
    Blynk.virtualWrite(2, metheoData.bmpTemperature);
    Blynk.virtualWrite(3, metheoData.shtHumidity);
    Blynk.virtualWrite(4, metheoData.bmpPresure);
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
