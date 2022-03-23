// Import required libraries
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "index.h"
#include "logout.h"


// Replace with your network credentials
const char* ssid = "yummy!!!";
const char* password = "rajput@123456";

// Web Server HTTP Authentication credentials
const char* http_username = "admin";
const char* http_password = "admin";

// Adafruit_BME280 bme;         // BME280 connect to ESP32 I2C (GPIO 21 = SDA, GPIO 22 = SCL)
const int buttonPin = 18;    // Pushbutton
const int ledPin = 19;       // Status LED
const int output = 32;       // Output socket
const int ldr = 33;          // LDR (Light Dependent Resistor)
const int motionSensor = 27; // PIR Motion Sensor

int ledState = LOW;           // current state of the output pin
int buttonState;              // current reading from the input pin
int lastButtonState = LOW;    // previous reading from the input pin
bool motionDetected = false;  // flag variable to send motion alert message
bool clearMotionAlert = true; // clear last motion alert message from web page

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncEventSource events("/events");

const char* PARAM_INPUT_1 = "state";

// Checks if motion was detected
void IRAM_ATTR detectsMovement() {
  //Serial.println("MOTION DETECTED!!!");
  motionDetected = true;
  clearMotionAlert = false;
}



String outputState(int gpio){
  if(digitalRead(gpio)){
    return "checked";
  }
  else {
    return "";
  }
}

String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons;
    String outputStateValue = outputState(32);
    buttons+="<div class=\"card card-switch\"><h4><i class=\"fas fa-lightbulb\"></i> OUTPUT</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"controlOutput(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label></div>";
    outputStateValue = outputState(19);
    buttons+="<div class=\"card card-switch\"><h4><i class=\"fas fa-lightbulb\"></i> STATUS LED</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleLed(this)\" id=\"led\" " + outputStateValue + "><span class=\"slider\"></span></label></div>";
    return buttons;
  }
  else if(var == "MOTIONMESSAGE"){
    if(!clearMotionAlert) {
      return String("<span style=\"color:#b30000;\">MOTION DETECTED!</span>");
    }
    else {
      return String("No motion");
    }
  }
  return String();
}