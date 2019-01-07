#include <Arduino.h>
#include <MetheoData.h>
#include <PowerController.h>
#include <InternetConnection.h>

MetheoData metheoData;
InternetConnection connection;
PowerController powerController;

// Deep sleep interval in minutes
const int deepSleepInterval = 2;

// Connections to APIs are OK
bool apisAreConnected = false;

void initializeInternetConnection()
{
    if (connection.initialize())
    {
        apisAreConnected = connection.initializeBlynk();
    }
}

void deepSleep()
{
    Serial.println("Good night");
    ESP.deepSleep(deepSleepInterval * 1000000 * 60);
}

// Set up environment before loop
void setup()
{
    Serial.begin(9600);
    initializeInternetConnection();
}

// Excecute code in forever loop
void loop()
{
    if (apisAreConnected)
    {
        metheoData.setData();
        powerController.setData();

        if (metheoData.dataAreValid())
        {
            connection.sendDataToBlynk(metheoData, powerController, true);
        }
        else
        {
            Serial.println("MetheoData are invalid");
            connection.sendDataToBlynk(metheoData, powerController, false);
        }
        connection.checkForUpdates();
        connection.runBlynk();
    }
    else
    {
        Serial.println("No connection to apis");
    }

    deepSleep();
}