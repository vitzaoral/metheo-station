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
    // TODO: nezapisovat spicky, napr teplota 2000000000 apod., v takovem pripade neposlat na API, nejakou fci ktera rekne true/false validni data v hranicich
    shtTemperature = 0;
    shtHumidity = 0;
    bmpPresure = bmp.readPressure() / 100.0;
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
