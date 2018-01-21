#include <Arduino.h>
#include <MetheoData.h>
#include <InternetConnection.h>

MetheoData metheoData;
InternetConnection connection;

// Set up environment before loop
void setup()
{
    // TODO: vyzkouset OTA
    Serial.begin(9600);
    connection.initializeThingSpeak();
    connection.initializeBlynk();
}

// Excecute code in forever loop
void loop()
{
    metheoData.setData();
    connection.setMeteoDataToThingSpeakObject(metheoData);
    connection.sendDataToThingSpeakApi();
    connection.sendDataToBlynk(metheoData);
   
    delay(15000);
}
