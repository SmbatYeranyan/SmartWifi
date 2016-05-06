/*
  SmartWifi.cpp - Library for seting up Wifi credentials using web browser for the ESP8266 wifi chip.
  Created by Smbat Yeranyan, May 5, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include "SmartWifi.h"

Pins pins;
SmartWifi::SmartWifi(Pins setPins)
{
  pins.resetButton = setPins.resetButton;
  pins.reset = setPins.reset;
  pins.error = setPins.error;
  pins.state = setPins.state;
}

String* SmartWifi::getWifiCredentials(){
  Serial.println("Reading EEPROM ssid");
  String* credentials = new String[2];
  for (int i = 0; i < 32; ++i)
    {
      credentials[0] += char(EEPROM.read(i));
    }
  Serial.print("SSID: ");
  Serial.println(credentials[0]);
  Serial.println("Reading EEPROM pass");
  
  for (int i = 32; i < 96; ++i)
    {
      credentials[1] += char(EEPROM.read(i));
    }
  Serial.print("PASS: ");
  Serial.println(credentials[1]); 
  return credentials;
}

String* SmartWifi::getDeviceId(){
  Serial.println("Reading EEPROM DeviceId");
  String* credentials = new String[1];
  for (int i = 96; i < 128; ++i)
    {
      credentials[0] += char(EEPROM.read(i));
    }
  Serial.print("DeviceId: ");
  Serial.println(credentials[0]);
  return credentials;
}

void SmartWifi::writeCredentials(String qsid, String qpass, String qid){

  Serial.println("clearing eeprom");
  for (int i = 0; i < 128; ++i) { EEPROM.write(i, 0); }
  Serial.println(qsid);
  Serial.println("");
  Serial.println(qpass);
  Serial.println("");      
  Serial.println(qid);
  Serial.println(qid.length());
    
  Serial.println("writing eeprom ssid:");
  for (int i = 0; i < qsid.length(); ++i)
    {
      EEPROM.write(i, qsid[i]);
      Serial.print("Wrote: ");
      Serial.println(qsid[i]); 
    }
  Serial.println("writing eeprom pass:"); 
  for (int i = 0; i < qpass.length(); ++i)
    {
      EEPROM.write(32+i, qpass[i]);
      Serial.print("Wrote: ");
      Serial.println(qpass[i]); 
    }   

  for (int i = 0; i < qid.length(); ++i)
    {
      EEPROM.write(96+i, qid[i]);
      Serial.print("Wrote: ");
      Serial.println(qid[i]); 
    }
  Serial.println("writing deviceId");  
  EEPROM.commit();
  content = "Rebooting...";
  statusCode = 200;
}
     
void clearEEPROM(){
  Serial.println("Button press");
  Serial.println("clearing eeprom");
  for (int i = 0; i < 200; ++i) { EEPROM.write(i, 0); }
  EEPROM.commit();
  delay(500);
  EEPROM.end();
}

void hardReboot(){
  digitalWrite(pins.reset, LOW);
}

bool testWifiConnection(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect");  
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { return true; } 
    delay(500);
    Serial.print(WiFi.status());    
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
} 