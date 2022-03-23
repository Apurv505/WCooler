/*********
  Complete project details at https://RandomNerdTutorials.com/esp32-iot-shield-pcb-dashboard/
*********/

#include "cooler0.h"



void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
    
  // if (!bme.begin(0x76)) {
  //   Serial.println("Could not find a valid BME280 sensor, check wiring!");
  //   while (1);
  // }
  
  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
  // initialize the LED pin as an output
  pinMode(output, OUTPUT);
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
   if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", logout_html, processor);
  });
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(401);
  });
  // Send a GET request to control output socket <ESP_IP>/output?state=<inputMessage>
  server.on("/output", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    String inputMessage;
    // GET gpio and state value
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      digitalWrite(output, inputMessage.toInt());
      request->send(200, "text/plain", "OK");
    }
    request->send(200, "text/plain", "Failed");
  });
  // Send a GET request to control on board status LED <ESP_IP>/toggle
  server.on("/toggle", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    request->send(200, "text/plain", "OK");
  });
  // Send a GET request to clear the "Motion Detected" message <ESP_IP>/clear-motion
  server.on("/clear-motion", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    clearMotionAlert = true;
    request->send(200, "text/plain", "OK");
  });
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis and set reconnect delay to 1 second
    client->send("hello!",NULL,millis(),1000);
  });
  server.addHandler(&events);
  
  // Start server
  server.begin();
}
 
void loop(){
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 10000;
  // read the state of the switch into a local variable
  int reading = digitalRead(buttonPin);

  // If the switch changed
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
        events.send(String(digitalRead(ledPin)).c_str(),"led_state",millis());
      }
    }
  }

  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    events.send("ping",NULL,millis());
    // events.send(String(bme.readTemperature()).c_str(),"temperature",millis());
    // events.send(String(bme.readHumidity()).c_str(),"humidity",millis());
    events.send(String(analogRead(ldr)).c_str(),"light",millis());
    lastEventTime = millis();
  }
  
  if(motionDetected & !clearMotionAlert){
    events.send(String("MOTION DETECTED!").c_str(),"motion",millis());
    motionDetected = false;
  }
  
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}


