/*********
  Complete project details at https://RandomNerdTutorials.com/esp32-iot-shield-pcb-dashboard/
*********/

#include "urls.h"

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinInit();

  // initialize the pushbutton pin as an input
  // pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output
  // pinMode(ledPin, OUTPUT);
  // // initialize the LED pin as an output
  // pinMode(output, OUTPUT);
  // // PIR Motion Sensor mode INPUT_PULLUP
  // pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  //   attachInterrupt(digitalPinToInterrupt(pinSense), detectsMovement, CHANGE);

  // Connect to Wi-Fi
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi..");
  // }
  // // Print ESP32 Local IP Address
  // Serial.println(WiFi.localIP());

  wifiInit();

  MDNS.begin("BlynkWCooler", WiFi.localIP());
  Serial.println("mDNS responder started");
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("wled", "tcp", 80);

  otaSetup();
  ArduinoOTA.begin();
  urlSetup();

  // EVENTS-----------------------
  events.onConnect([](AsyncEventSourceClient *client)
                   {
                     if (client->lastId())
                     {
                       Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
                     }
                     // send event with message "hello!", id current millis and set reconnect delay to 1 second
                    client->send("hello!", NULL, millis(), 1000); 
                    events.send(String(fanSpeed).c_str(), "fan", millis());
                    events.send(String(pumpSpeed).c_str(), "pump", millis());
                    events.send(String(direction).c_str(), "direction", millis()); });

  server.addHandler(&events);

  // Start server
  server.begin();
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------//
void loop()
{
  MDNS.update();
  ArduinoOTA.handle();
  Blynk.run();

  // if ((millis() - lastEventUpdateTime) > eventUpdateInterval)
  // {
  //   events.send(String(fanSpeed).c_str(), "fan", millis());
  //   events.send(String(pumpSpeed).c_str(), "pump", millis());
  //   events.send(String(direction).c_str(), "direction", millis());

  //   lastEventUpdateTime = millis();
  // }

  //   if ((millis() - lastEventUpdateTime) > eventUpdateInterval)
  // {
  //   Blynk.virtualWrite(V0, fanSpeed);
  //   Blynk.virtualWrite(V1, pumpSpeed);
  //   Blynk.virtualWrite(V2, direction);
  //   lastEventUpdateTime = millis();
  // }

  // events.send(String(bme.readTemperature()).c_str(),"temperature",millis());
  // static unsigned long lastEventTime = millis();
  // static const unsigned long EVENT_INTERVAL_MS = 10000;
  // read the state of the switch into a local variable

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (digitalRead(buttonPin) == LOW)
    {
      Serial.println("Button pressed");
      events.send("On", "light", millis());
      lastDebounceTime = millis();
    }
  }

  //  }

  // If the switch changed
  // if (reading != lastButtonState) {
  //   // reset the debouncing timer
  //   lastDebounceTime = millis();
  // }

  // if ((millis() - lastDebounceTime) > debounceDelay) {
  //   // if the button state has changed:
  //   if (reading != buttonState) {
  //     buttonState = reading;
  //     // only toggle the LED if the new button state is HIGH
  //     if (buttonState == HIGH) {
  //       ledState = !ledState;
  //       digitalWrite(ledPin, ledState);
  //       events.send(String(digitalRead(ledPin)).c_str(),"led_state",millis());
  //     }
  //   }
  // }

  // if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
  //   events.send("ping",NULL,millis());
  // events.send(String(bme.readTemperature()).c_str(),"temperature",millis());
  //   // events.send(String(bme.readHumidity()).c_str(),"humidity",millis());
  //   events.send(String(analogRead(ldr)).c_str(),"light",millis());
  //   lastEventTime = millis();
  // }

  // if(motionDetected & !clearMotionAlert){
  //   events.send(String("MOTION DETECTED!").c_str(),"motion",millis());
  //   motionDetected = false;
  // }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  // lastButtonState = reading;
}
