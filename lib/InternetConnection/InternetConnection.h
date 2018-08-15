#ifndef __InternetConnection_H
#define __InternetConnection_H

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <MetheoData.h>
#include <PowerController.h>

class InternetConnection
{
public:
  bool initialize(void);
  bool initializeBlynk(void);
  void sendDataToBlynk(MetheoData, PowerController, bool);
  void setStatusToBlynk(bool, String, int);
  void checkForUpdates(void);
  void runBlynk();
};

#endif