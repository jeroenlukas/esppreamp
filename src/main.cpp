#include <Arduino.h>

#include <AsyncTCP.h>
#include <WiFi.h>

#include <Wire.h>
#include <ESPAsyncWebServer.h>
#include <Ticker.h>

#include "defines.h"
#include "config.h"

#include "adau1701/adau1701.h"

#include "hmi/cli.h"
#include "filesystem/filesystem.h"
#include "models.h"
#include "patches.h"

static AsyncWebServer server(80);

bool level_detect_in = false;

bool flag_level_in = false;
Ticker ticker_level_ref;

void ticker_level()
{
  flag_level_in = true;
}

void setup() 
{
  Serial.begin(115200);

  delay(500);

  Serial.println("*** ESPPreamp v" + String(EP_VERSION) + " ***");

  // Init I2C
  Wire.begin();

  // Init CLI
  cli_init();

  // Init filesystem
  filesystem_init();

  // Init ADAU1701
  adau1701_init();
  
  Serial.println("Connecting to " + String(WIFI_SSID) + "...");
  WiFi.mode(WIFI_STA);  
  WiFi.disconnect();
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

  // Load patch 1 by default
  patch_activate(1);

  ticker_level_ref.attach(0.2, ticker_level);

}

// not needed
void loop() 
{ 
  cli_handle();

  //adau1701_readsignalin();
  if(level_detect_in && flag_level_in)
  {
    flag_level_in = false;
    int32_t input = adau1701_read_signal_in();
    Serial.println("IN: "  + String(input));
  }

}