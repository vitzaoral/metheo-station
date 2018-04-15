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

/*
 * Voltage divider of 100k+220k over 100k
 * gives 100/420k
 * ergo 4.2V -> 1Volt
 * Max input on A0=1Volt ->1023
 * 4.2*(Raw/1023)=Vbat
 */
float readBatteryVoltage()
{
    int raw = analogRead(A0);
    float volt = raw / 1023.0;
    volt = volt * 4.2;
    return volt;
}

// Set up environment before loop
void setup()
{
    // TODO: vyzkouset OTA
    Serial.begin(9600);
    pinMode(A0, INPUT);
    Serial.println("");
    initializeInternetConnection();
}

// Excecute code in forever loop
void loop()
{
    if (apisAreConnected)
    {
        metheoData.setData();
        float batteryVoltage = readBatteryVoltage();
        if (metheoData.dataAreValid())
        {
            connection.setMeteoDataToThingSpeakObject(metheoData, batteryVoltage);
            connection.sendDataToThingSpeakApi();
            connection.sendDataToBlynk(metheoData, batteryVoltage, true);
        }
        else
        {
            Serial.println("MetheoData are invalid");
            connection.sendDataToBlynk(metheoData, batteryVoltage, false);
        }
    }
    else
    {
        Serial.println("No connection to apis");
    }

    deepSleep();
}
