#include "cooler.h"



void urlSetup(){
    server.on("/win", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Device being searched via mDNS dicovery");
    request->send_P(200, "text/html", win_xml);
    });
    

server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });
  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", logout_html); });
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(401); });

  // Send a GET request to control output socket <ESP_IP>/output?state=<fanSpeed>
  server.on("/output", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // if(!request->authenticate(http_username, http_password))
    //   return request->requestAuthentication();
   
    if (request->hasParam("fan")) {
      fanSpeed = request->getParam("fan")->value().toInt();
      updateSpeed();
      Blynk.virtualWrite(V0, fanSpeed);
      request->send(200, "text/plain", "OK");
      
    }
    
    if (request->hasParam("pump")) {
      pumpSpeed = request->getParam("pump")->value().toInt();
      updatePump();
      Blynk.virtualWrite(V1, pumpSpeed);
      request->send(200, "text/plain", "OK");
    }

    if (request->hasParam("direction")) {
      direction = request->getParam("direction")->value().toInt();
      updateDirection();
      Blynk.virtualWrite(V2, direction);  
      request->send(200, "text/plain", "OK");
    }
    request->send(200, "text/plain", "Failed"); });
  

    
}

