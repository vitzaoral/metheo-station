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
    WiFi.begin(ssid, password);

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
    // TODO: problem, pokud mam spatny klic (pokud nefunguje Blynk tak mozna ne)
    Serial.println("WiFi connecting to Blynk");
    Blynk.begin(blynkAuth, ssid, password);
    Blynk.run();
    
    Serial.println(Blynk.connected() ? "Blynk connected" : "Timeout on Blynk");
    return Blynk.connected();
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
    // create data to send to Blynk
    if (Blynk.connect())
    {
        Blynk.virtualWrite(1, metheoData.shtTemperature);
        Blynk.virtualWrite(2, metheoData.bmpTemperature);
        Blynk.virtualWrite(3, metheoData.shtHumidity);
        Blynk.virtualWrite(4, metheoData.bmpPresure);
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
