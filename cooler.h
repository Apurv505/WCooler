// Import required libraries
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "index.h"
#include "logout.h"
#include "xml.h"
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>


// Replace with your network credentials
const char *ssid = "M";
const char *password = "";

// Web Server HTTP Authentication credentials
const char *http_username = "admin";
const char *http_password = "admin";

unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 500;  // the debounce time; increase if the output flickers
unsigned long lastEventUpdateTime = 0;
unsigned long eventUpdateInterval = 1000;

const int speedLow = 5;
const int speedMid = 4;
const int speedHigh = 0;
const int pump = 2;
const int buttonPin = 13;
const int ledPin = 16;

int fanSpeed = 0;
int pumpSpeed = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncEventSource events("/events");


String outputState(int gpio)
{
  if (digitalRead(gpio))
  {
    return "checked";
  }
  else
  {
    return "";
  }
}

void pinInit()
{
  pinMode(speedLow, OUTPUT);
  pinMode(speedMid, OUTPUT);
  pinMode(speedHigh, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  digitalWrite(speedLow, HIGH); // Off on HIGH logic. Inverted logic of ESP8266
  digitalWrite(speedMid, HIGH); // Turns off all relays at restart
  digitalWrite(speedHigh, HIGH);
  digitalWrite(ledPin, HIGH);
  digitalWrite(pump, !pumpSpeed);
}

void otaSetup(){
    // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname("EspWCooler");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
}

  String processor(const String &var)
  {
    // Serial.println(var);
    if (var == "BUTTONPLACEHOLDER")
    {
      String buttons;
      String outputStateValue = outputState(32);
      buttons += "<div class=\"card card-switch\"><h4><i class=\"fas fa-lightbulb\"></i> OUTPUT</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"controlOutput(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label></div>";
      outputStateValue = outputState(19);
      buttons += "<div class=\"card card-switch\"><h4><i class=\"fas fa-lightbulb\"></i> STATUS LED</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleLed(this)\" id=\"led\" " + outputStateValue + "><span class=\"slider\"></span></label></div>";
      return buttons;
    }
    // else if(var == "MOTIONMESSAGE"){
    //   if(!clearMotionAlert) {
    //     return String("<span style=\"color:#b30000;\">MOTION DETECTED!</span>");
    //   }
    //   else {
    //     return String("No motion");
    //   }
    // }
    return String();
  }
