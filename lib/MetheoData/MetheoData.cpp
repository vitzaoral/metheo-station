#include "MetheoData.h"

Adafruit_BME280 bme;

// Initialize and get metheorological data
MetheoData::MetheoData()
{
    // BME/BMP280 has I2C address 0x76. GY-BMP208-3.3 has default address.
    if (!bme.begin(0x76))
    {
        Serial.println("Could not find a valid temperature sensor!");
    }
}

void MetheoData::setData(void)
{
    presure = bme.readPressure() / 100.0;
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();

    Serial.print("Presure: ");
    Serial.println(presure);
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
}

bool MetheoData::dataAreValid(void)
{
    return temperature <= 100.0 && temperature >= -100.0 &&
           humidity <= 100.0 && humidity >= 0.0 && presure > 700;
}
