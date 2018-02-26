#include <Arduino.h>
#include <MetheoData.h>
#include <InternetConnection.h>

MetheoData metheoData;
InternetConnection connection;

// Deep sleep interval in minutes
const int deepSleepInterval = 1;

// Connections to APIs are OK
bool apisAreConnected = false;

void initializeInternetConnection()
{
    if (connection.initializeThingSpeak())
    {
        apisAreConnected = connection.initializeBlynk();
    }
}

void deepSleep()
{
    Serial.println("Good night");
    Serial.println("");
    ESP.deepSleep(deepSleepInterval * 1000000 * 60);
}

// Set up environment before loop
void setup()
{
    // TODO: vyzkouset OTA
    Serial.begin(9600);
    Serial.println("");
    initializeInternetConnection();
}

// Excecute code in forever loop
void loop()
{
    if (apisAreConnected)
    {
        metheoData.setData();
        if (metheoData.dataAreValid())
        {
            connection.setMeteoDataToThingSpeakObject(metheoData);
            connection.sendDataToThingSpeakApi();
            connection.sendDataToBlynk(metheoData);
        }
        else {
            Serial.println("MetheoData are invalid");
        }
    }
    else
    {
        Serial.println("No connection to apis");
    }

    deepSleep();
}
