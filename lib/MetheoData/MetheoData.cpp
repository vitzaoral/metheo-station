#include "MetheoData.h"

SHT3X sht30(0x45);
Adafruit_BMP085 bmp;

// Initialize and get metheorological data
MetheoData::MetheoData()
{
    if (!bmp.begin())
    {
        Serial.println("Could not find a valid BMP100 sensor on oaddress 0x45!");
    }
}

void MetheoData::setData(void)
{
    shtTemperature = 1000;
    shtHumidity = 1000;
    bmpPresure = bmp.readPressure() / 100.0;
    bmpTemperature = bmp.readTemperature();
    // bmpRealAltitude = bmp.readAltitude(101500);

    if (sht30.get() == 0)
    {
        shtTemperature = sht30.cTemp;
        shtHumidity = sht30.humidity;
    }
    else
    {
        Serial.println("SHT30 sensor error!");
    }
}

bool MetheoData::dataAreValid(void)
{
    return shtTemperature <= 100.0 && shtTemperature >= -100.0 &&
           shtHumidity <= 100.0 && shtHumidity >= 0.0 &&
           bmpPresure <= 1200.0 && bmpPresure >= 800.0 &&
           bmpTemperature <= 100.0 && bmpTemperature >= -100.0;
}
