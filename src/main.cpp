#include <Arduino.h>
#include <MetheoData.h>
#include <OledDisplay.h>


MetheoData metheoData;
OledDisplay oledDisplay;

// Set up environment before loop
void setup()
{
    Serial.begin(9600);
}

// Excecute code in forever loop
void loop()
{
    metheoData.setData();
    oledDisplay.printMetheoDataToDisplay(metheoData);
    delay(3000);
}