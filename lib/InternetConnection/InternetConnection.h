#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <MetheoData.h>
#include <PowerController.h>

class InternetConnection
{
public:
  bool initializeThingSpeak(void);
  bool initializeBlynk(void);
  void setMeteoDataToThingSpeakObject(MetheoData, PowerController);
  bool sendDataToThingSpeakApi(void);
  void sendDataToBlynk(MetheoData, PowerController, bool);
  void setStatusToBlynk(bool, String, int);
  void checkForUpdates(void);
  void runBlynk();
};

#endif