#include <Arduino.h>
#include <MetheoData.h>
#include <OledDisplay.h>
#include <InternetConnection.h>

MetheoData metheoData;
OledDisplay oledDisplay;
InternetConnection connection;

// Set up environment before loop
void setup()
{
    // TODO: vyzkouset OTA
    // TODO: nejdrive ziskat teplotu a az pak inicializovat wifi...ale asi neresit pro pripad deepSleep
    Serial.begin(9600);
    delay(100);
    connection.initialize();
}

// Excecute code in forever loop
void loop()
{
    metheoData.setData();
    oledDisplay.printMetheoDataToDisplay(metheoData);
    connection.setMeteoDataToThingSpeakObject(metheoData);
    connection.sendDataToThingSpeakApi();

    // TODO: co kdyz chci zobrazovat data co 10 sekund ale posilat je co 1 minutu
    // pouzit millis() nebo ticker.h
    delay(60000);
}
