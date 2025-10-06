#include <Arduino.h>

#include <AsyncTCP.h>
#include <WiFi.h>

#include <Wire.h>
#include <ESPAsyncWebServer.h>

#include "config.h"

#include "adau1701/adau1701.h"

#include "hmi/cli.h"
#include "filesystem/filesystem.h"
#include "models.h"

static AsyncWebServer server(80);



void setup() 
{
  Serial.begin(9600);

  delay(500);

  // Init I2C
  Wire.begin();

  // Init CLI
  cli_init();

  // Init filesystem
  filesystem_init();

  // Init ADAU1701
  adau1701_init();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
      Serial.print('.');
      delay(500);
  }

  Serial.println("IP: " + WiFi.localIP().toString());

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