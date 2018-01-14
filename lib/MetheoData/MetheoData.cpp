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
    shtTemperature = 0;
    shtHumidity = 0;
    bmpPresure = bmp.readPressure();
    bmpTemperature = bmp.readTemperature();
    bmpRealAltitude = bmp.readAltitude(101500);

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
