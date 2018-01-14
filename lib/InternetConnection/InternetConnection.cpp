#include "InternetConnection.h"
#include "../../src/settings.cpp"

WiFiClient client;
Settings settings;

const char *thingSpeakWriteApiKey = settings.thingSpeakWriteApiKey;
const unsigned long thingSpeakChannelId = settings.thingSpeakChannelId;
const char *ssid = settings.ssid;
const char *password = settings.password;
const char *server = settings.server;

// Initialize WiFi connection and ThingSpeak
void InternetConnection::initialize(void)
{
    WiFi.begin(ssid, password);

    Serial.print("WiFi connecting to: ");
    Serial.println(ssid);
    // TODO: resit jen nekolik poctu pripojeni pokud WIFI neni at nejede while donekonecna
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    ThingSpeak.begin(client);
}

void InternetConnection::setMeteoDataToThingSpeakObject(MetheoData metheoData)
{
    // create data to send to ThingSpeak
    ThingSpeak.setField(1, metheoData.shtTemperature);
    ThingSpeak.setField(2, metheoData.bmpTemperature);
    ThingSpeak.setField(3, metheoData.bmpPresure);
    ThingSpeak.setField(4, metheoData.shtHumidity);
}

bool InternetConnection::sendDataToThingSpeakApi(void)
{
    // Send data in one API call
    // TODO: co se stane pokud sluzba zrovna nejede, vypadne WIFi apod.
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
