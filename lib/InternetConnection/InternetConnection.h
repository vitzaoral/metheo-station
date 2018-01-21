#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <MetheoData.h>

class InternetConnection
{
public:
  bool initializeThingSpeak(void);
  bool initializeBlynk(void);
  void setMeteoDataToThingSpeakObject(MetheoData);
  bool sendDataToThingSpeakApi(void);
  void sendDataToBlynk(MetheoData);
};

#endif