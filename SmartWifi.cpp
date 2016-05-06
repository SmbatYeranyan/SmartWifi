/*
  SmartWifi.cpp - Library for seting up Wifi credentials using web browser for the ESP8266 wifi chip.
  Created by Smbat Yeranyan, May 5, 2016.
  Released into the public domain.
*/

#include "Arduino.h"
#include "SmartWifi.h"

ESP8266WebServer server(80);
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
     
void SmartWifi::clearEEPROM(){
  Serial.println("Button press");
  Serial.println("clearing eeprom");
  for (int i = 0; i < 200; ++i) { EEPROM.write(i, 0); }
  EEPROM.commit();
  delay(500);
  EEPROM.end();
}

void SmartWifi::hardReboot(){
  digitalWrite(pins.reset, LOW);
}

bool SmartWifi::testWifiConnection(void) {
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

void SmartWifi::launchWeb(int webtype) {
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer(webtype);
  // Start the server
  server.begin();
  Serial.println("Server started"); 
}

void SmartWifi::setupAP(void) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
     }
  }
  Serial.println(""); 
  st = "";
  for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      st += "<option value='"+WiFi.SSID(i)+"'>";
      st += WiFi.SSID(i);
      //st += " (";
     // st += WiFi.RSSI(i);
      //st += ")";
      //st += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
      st += "</option>";
    }
 
  delay(100);
  WiFi.softAP("ZakuAI");
  Serial.println("softap");
  launchWeb(1);
  Serial.println("over");
}

void SmartWifi::createWebServer(int webtype){

  if ( webtype == 1 ) {
    server.on("/", []() {
        content="";
        IPAddress ip = WiFi.softAPIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        //content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
        //content += ipStr;
        //content += "<p>";
        //content += st;
        content += "<!DOCTYPE html> <html > <head> <meta charset='UTF-8'> <title>ZakuAi Setup</title> <meta name='viewport' content='width=device-width, initial-scale=1.0'>";
        content += "</head> <style> * { box-sizing: border-box; } html { height: 100%; } body { background-color: #2c3338; color: #606468; font: 400 0.875rem/1.5 'Open Sans', sans-serif; margin: 0";
        content += "min-height: 100%; } a { color: #eee; outline: 0; text-decoration: none; } a:focus, a:hover { text-decoration: underline; } h1{ text-align: center; font-size: 21px; color: #B5B5B5; } input { border: 0; color: inherit; font: inherit; margin: 0; outline: 0; padding: 0; -webkit-transition: background-color .3s; transition: background-color .3s; } .site__container { -webkit-box-flex: 1; -webkit-flex: 1; -ms-flex: 1; flex: 1; padding: 3rem 0; } .form input[type='password'], .form input[type='text'], .form input[type='submit'], select { width: 100%; } .form--login { color: #606468; } .form--login label, .form--login input[type='text'], .form--login input[type='password'], .form--login select, .form--login input[type='submit'] { border-radius: 0.25rem; padding: 1rem; } .form--login label { background-color: #363b41; border-bottom-right-radius: 0; border-top-right-radius: 0; padding-left: 1.25rem; padding-right: 1.25rem; } .form--login input[type='text'], .form--login input[type='password'], select { background-color: #3b4148; border-bottom-left-radius: 0; border-top-left-radius: 0; } select,.form--login input[type='text']:focus, .form--login input[type='text']:hover, .form--login input[type='password']:focus, .form--login input[type='password']:hover { background-color: #434A52; } .form--login input[type='submit'] { background-color: #ea4c88; color: #eee; font-weight: bold; text-transform: uppercase; } .form--login input[type='submit']:focus, .form--login input[type='submit']:hover { background-color: #d44179; } .form__field { display: -webkit-box; display: -webkit-flex; display: -ms-flexbox; display: flex; margin-bottom: 1rem; } .form__input { -webkit-box-flex: 1; -webkit-flex: 1; -ms-flex: 1; flex: 1; } .align { -webkit-box-align: center; -webkit-align-items: center; -ms-flex-align: center; align-items: center; display: -webkit-box; display: -webkit-flex; display: -ms-flexbox; display: flex; -webkit-box-orient: horizontal; -webkit-box-direction: normal; -webkit-flex-direction: row; -ms-flex-direction: row; flex-direction: row; } select{ color:white; border:none; outline: none; color: #ABABAB; } option{ background-color:#434A52; color:#ABABAB; } .hidden { border: 0; clip: rect(0 0 0 0); height: 1px; margin: -1px; overflow: hidden; padding: 0; position: absolute; width: 1px; } .text--center { text-align: center; } .grid__container { margin: 0 auto; max-width: 20rem; width: 90%; } </style> <body> <body class='align'> <div class='site__container'> <div class='grid__container'> <form method='get' action='setting' class='form form--login'> <h1>ZakuAi Wifi Setup</h1> <div class='form__field' id='ssidlist'> <select name='ssid'>"+st+" </select> </div> <div class='form__field'> <input name='pass' id='login__password' type='password' class='form__input' placeholder='Password' required> </div> <div class='form__field'> <input name='device' id='login__password' type='text' class='form__input' placeholder='DeviceId' required> </div> <div class='form__field'> <input type='submit' value='Connect'> </div> </form> <p class='text--center'>SSID not found? <a onclick='showSSID()'>Enter Manually</a> <span class='fontawesome-arrow-right'></span></p> </div> </div> </body> </body> </html> <script> function showSSID(){ document.getElementById('ssidlist').style.display = 'none'; document.getElementById('ssid').style.display = 'block'; } </script>";
        //content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
         //content += "</html>";
        server.send(200, "text/html", content);  
    });
    server.on("/setting", []() {
        String qsid = server.arg("ssid");
        String qpass = server.arg("pass");
        String qid = server.arg("device");

        if (qsid.length() > 0 && qpass.length() > 0) {
          smartWifi.writeCredentials(qsid, qpass, qid);
   
        } else {
          content = "{\"Error\":\"404 not found\"}";
          statusCode = 404;
          Serial.println("Sending 404");
        }
        server.send(statusCode, "application/json", content);
        WiFi.disconnect();
        delay(2000);
        EEPROM.end();
        smartWifi.resetBoard();
    });
  } else if (webtype == 0) {
    server.on("/", []() {
      IPAddress ip = WiFi.localIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      server.send(200, "application/json", "{\"IP\":\"" + ipStr + "\"}");
    });
    server.on("/cleareeprom", []() {
      content = "<!DOCTYPE HTML>\r\n<html>";
      content += "<p>Clearing the EEPROM</p></html>";
      server.send(200, "text/html", content);
      smartWifi.clearEEPROM();
      smartWifi.resetBoard();
    });
  }
}