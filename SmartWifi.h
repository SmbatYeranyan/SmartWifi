/*
  SmartWifi.h - Library for seting up Wifi credentials using web browser for the ESP8266 wifi chip.
  Created by Smbat Yeranyan, May 5, 2016.
  Released into the public domain.
*/


#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
struct Pins {
  int resetButton = 12;
  int reset = 5;
  int error = 14;
  int state = 16;
};
class SmartWifi
{
  public:
  	SmartWifi();
  	void init(Print &print, int resetButton, int reset, int error, int state);
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
    void resetButton();
    String deviceId;
	bool setupMode = false;

  private:
	WiFiClient client;
	const char* ssid;
	const char* passphrase;
	String st;
	String content;
	String apName = "SmartWifi";
    String statusCode;
    String uiTitle ="SmartWifi";
    Pins pins;
    Print* printer;

	unsigned long previousMillis = 0;        // will store last time LED was updated
	const long interval = 35;
	unsigned long previousMillisReboot = 0;        // will store last time LED was updated
	long intervalReboot = 5000000;
	int brightness = 0;    // how bright the LED is
	int fadeAmount = 5;
};
