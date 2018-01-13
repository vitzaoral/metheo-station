#ifndef __MetheoData_H
#define __MetheoData_H

#include <Arduino.h>
#include <WEMOS_SHT3X.h>
#include <Adafruit_BMP085.h>

class MetheoData
{
public:
  MetheoData();
  float shtTemperature;
  float shtHumidity;
  float bmpPresure;
  float bmpTemperature;
  float bmpRealAltitude;

  void setData(void);
};

#endif
