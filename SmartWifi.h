/*
  SmartWifi.h - Library for seting up Wifi credentials using web browser for the ESP8266 wifi chip.
  Created by Smbat Yeranyan, May 5, 2016.
  Released into the public domain.
*/


#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

struct Pins{
  int resetButton = 12;
  int reset = 5;
  int error = 14;
  int state = 16;
};

class SmartWifi
{
  public:
  	SmartWifi();
  	void init(Print &print);
    void SetupWifi();
    String* getWifiCredentials();
    String* getDeviceId();
    void writeCredentials(String qsid, String qpass, String qid);
    void clearEEPROM();
    void hardReboot();
    bool testWifiConnection();
    void setupAP();
    void createWebServer(int webtype);
    void launchWeb(int webtype);
    void handleCycle();
    
  private:
	WiFiClient client;
	bool setupMode = false;
	String deviceId;
	const char* ssid;
	const char* passphrase;
	String st;
	String content;
    String statusCode;
    String uiTitle ="SmartWifi";
    void resetButton();
    Pins pins;
    Print* printer;
};
