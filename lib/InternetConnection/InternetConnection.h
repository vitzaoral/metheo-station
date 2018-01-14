#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <MetheoData.h>

class InternetConnection
{
public:
  void initialize(void);
  void setMeteoDataToThingSpeakObject(MetheoData);
  bool sendDataToThingSpeakApi(void);
};

#endif