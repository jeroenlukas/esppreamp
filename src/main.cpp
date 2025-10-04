// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright 2016-2025 Hristo Gochkov, Mathieu Carbou, Emil Muratov

//
// https://github.com/ESP32Async/ESPAsyncWebServer/discussions/23
//

#include <Arduino.h>
//#if defined(ESP32) || defined(LIBRETINY)
#include <AsyncTCP.h>
#include <WiFi.h>
/*#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(TARGET_RP2040) || defined(TARGET_RP2350) || defined(PICO_RP2040) || defined(PICO_RP2350)
#include <RPAsyncTCP.h>
#include <WiFi.h>
#endif*/
#include <Wire.h>
#include <ESPAsyncWebServer.h>

#include "adau1701/adau1701.h"

#include "hmi/cli.h"

static AsyncWebServer server(80);



void setup() 
{
  Serial.begin(9600);

  delay(1000);
  Serial.print("HOI");

  // Init I2C
  Wire.begin();

  // Init CLI
  cli_init();

  // Init ADAU1701
  adau1701_init();
  

//#if SOC_WIFI_SUPPORTED || CONFIG_ESP_WIFI_REMOTE_ENABLED || LT_ARD_HAS_WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin("Rinus", "scheldestraat");
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print('.');
      delay(500);
  }

  Serial.println("IP: " + WiFi.localIP().toString());

//#endif

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello, worldddddd!");
    Serial.println("sdsdf");
   
  });

  server.begin();
  Serial.println("begin");


}

// not needed
void loop() 
{

  
  cli_handle();

}