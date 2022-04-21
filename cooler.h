// Import required libraries
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "index.h"
#include "logout.h"
#include "xml.h"
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

char auth[] = "E5RKLm_V1XVXtqypJQ2UaTWrWnzlHVOP"; // Put Your Auth Token Here

const char *KNOWN_SSID[] = {"M", "yummy!!!"};         // Put all Your WiFi Network Names
const char *KNOWN_PASSWORD[] = {"", "rajput@123456"}; // Put the WiFi Passwords in same order. For Open networks leave the password blank inside the double quotes.

const int KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]); // number of known networks



// Replace with your network credentials
// const char *ssid = "yummy!!!";
// const char *password = "rajput@123456";

// Web Server HTTP Authentication credentials
const char *http_username = "admin";
const char *http_password = "admin";

unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 500;  // the debounce time; increase if the output flickers
unsigned long lastEventUpdateTime = 0;
unsigned long eventUpdateInterval = 500;

const int directionPin = 0;
const int speedMid = 5;
const int speedHigh = 4;
const int pumpPin = 2;
const int buttonPin = 13;
const int ledPin = 16;

int fanSpeed = 0;
int pumpSpeed = 0;
int direction = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncEventSource events("/events");


void wifiInit(){
  boolean wifiFound = false;
int i, n;

// ----------------------------------------------------------------
// Set WiFi to station mode and disconnect from an AP if it was previously connected
// ----------------------------------------------------------------
WiFi.mode(WIFI_STA);
WiFi.disconnect();
delay(100);
Serial.println("Setup done");

// ----------------------------------------------------------------
// WiFi.scanNetworks will return the number of networks found
// ----------------------------------------------------------------
Serial.println(F("Scan start"));
int nbVisibleNetworks = WiFi.scanNetworks();
Serial.println(F("Scan Completed"));
if (nbVisibleNetworks == 0) {
Serial.println(F("No networks found. Reset to try again"));
while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
}

// ----------------------------------------------------------------
// if you arrive here at least some networks are visible
// ----------------------------------------------------------------
Serial.print(nbVisibleNetworks);
Serial.println(" network(s) found");

// ----------------------------------------------------------------
// check if we recognize one by comparing the visible networks
// one by one with our list of known networks
// ----------------------------------------------------------------

for (i = 0; i < nbVisibleNetworks; ++i) {
Serial.println(WiFi.SSID(i)); // Print current SSID
for (n = 0; n < KNOWN_SSID_COUNT; n++) { // walk through the list of known SSID and check for a match
if (strcmp(KNOWN_SSID[n], WiFi.SSID(i).c_str())) {
Serial.print(F("\tNot matching "));
Serial.println(KNOWN_SSID[n]);
} else { // we got a match
wifiFound = true;
break; // n is the network index we found
}
} // end for each known wifi SSID
if (wifiFound) break; // break from the "for each visible network" loop
}

if (!wifiFound) {
Serial.println(F("No Known Network identified. Reset to try again"));
while (true);

}

const char* ssid = (KNOWN_SSID[n]);
const char* pass = (KNOWN_PASSWORD[n]);
Serial.println(WiFi.localIP());
Blynk.begin(auth, ssid, pass);
Serial.println("Blynk Connected"); //Connected and Authenticated with Blynk Server
}

void pinInit()
{
  pinMode(directionPin, OUTPUT);
  pinMode(speedMid, OUTPUT);
  pinMode(speedHigh, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  digitalWrite(directionPin, HIGH); // Off on HIGH logic. Inverted logic of ESP8266
  digitalWrite(speedMid, HIGH); // Turns off all relays at restart
  digitalWrite(speedHigh, HIGH);
  digitalWrite(ledPin, HIGH);
  digitalWrite(pumpPin, !pumpSpeed);
}

void otaSetup(){
    // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname("BlynkEspWCooler");

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

void updateSpeed(){
      if(fanSpeed == 0){
        digitalWrite(speedMid, HIGH);
        digitalWrite(speedHigh,HIGH);
        digitalWrite(ledPin, HIGH);
        events.send(String(fanSpeed).c_str(), "fan", millis());
        Serial.println("Got speed 0");
        
      
      }
      else if (fanSpeed == 1)
      {
        digitalWrite(speedMid, HIGH);
        digitalWrite(speedHigh, LOW);
        digitalWrite(ledPin, LOW);
        events.send(String(fanSpeed).c_str(), "fan", millis());
        Serial.println("Got speed 1");
        
      }
      else if (fanSpeed == 2)
      {
        digitalWrite(speedMid, LOW);
        digitalWrite(speedHigh, HIGH);
        digitalWrite(ledPin, LOW);
        events.send(String(fanSpeed).c_str(), "fan", millis());
        Serial.println("Got speed 2");
        
      }
      // else if (fanSpeed == 3)
      // {
      //   digitalWrite(speedMid, HIGH);
      //   digitalWrite(speedHigh, LOW);
      //   digitalWrite(ledPin, LOW);
      //   Serial.println("Got speed 3");
      //   request->send(200, "text/plain", "OK");
      // }
}



void updatePump(){
digitalWrite(pumpPin, !pumpSpeed);
      events.send(String(pumpSpeed).c_str(), "pump", millis());
}

void updateDirection(){
  digitalWrite(directionPin, !direction);
      events.send(String(direction).c_str(), "direction", millis());
}

BLYNK_CONNECTED()
{
  // Request Blynk server to re-send latest values for all pins
  Blynk.syncAll();

  fanSpeed= V0;
  pumpSpeed = V1;
  direction = V2;
  updateSpeed();
  updatePump();
  updateDirection();

  // You can also update individual virtual pins like this:
  // Blynk.syncVirtual(V0, V2);

  // Let's write your hardware uptime to Virtual Pin 2
  
}

BLYNK_WRITE(V0)
{
  // Use of syncAll() will cause this function to be called
  // Parameter holds last slider value
  fanSpeed = param.asInt();
  updateSpeed();
  Serial.printf("Got fan speed %d from Blynk\n", fanSpeed);
}

BLYNK_WRITE(V1)
{
  // Use of syncAll() will cause this function to be called
  // Parameter holds last slider value
  pumpSpeed = param.asInt();
  updatePump();
  Serial.printf("Got pump speed %d from Blynk\n", pumpSpeed);
}

BLYNK_WRITE(V2)
{
  // You'll get uptime value here as result of syncAll()
  direction = param.asInt();
  updateDirection();
  Serial.printf("Got direction %d from Blynk\n", direction);
}