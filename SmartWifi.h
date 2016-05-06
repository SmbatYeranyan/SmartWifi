/*
  SmartWifi.h - Library for seting up Wifi credentials using web browser for the ESP8266 wifi chip.
  Created by Smbat Yeranyan, May 5, 2016.
  Released into the public domain.
*/
#ifndef SmartWifi_h
#define SmartWifi_h

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>


struct Pins{
  int resetButton;
  int reset;
  int error;
  int state;
};

class SmartWifi
{
  public:
  	SmartWifi(Pins pins);

    String* getWifiCredentials();
    String* getDeviceId();

    void writeCredentials(String qsid, String qpass, String qid);
    void clearEEPROM();
    void hardReboot();
    bool testWifiConnection();

  private:
    int _pin;
    String content;
    String statusCode;
    Pins pins;
};

#endif