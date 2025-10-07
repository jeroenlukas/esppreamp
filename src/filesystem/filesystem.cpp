#include <Arduino.h>
#include <LittleFS.h>

#include "filesystem.h"

#define FORMAT_LITTLEFS_IF_FAILED   true

void filesystem_init();

void filesystem_init()
{
    Serial.println("Filesystem init");
    if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        Serial.println("LittleFS mounting failed!");
        return;
    }
    Serial.println("LittleFS mounted OK");

    // Check if required yaml files exist
    if(!LittleFS.exists("/config.yaml"))
        Serial.println("WARNING: /config.yaml does not exist!");

    if(!LittleFS.exists("/models.yaml"))
        Serial.println("WARNING: /models.yaml does not exist!");
    
    if(!LittleFS.exists("/patches.yaml"))
        Serial.println("WARNING: /patches.yaml does not exist!");
}

String filesystem_readfile(String filename)
{
    File file = LittleFS.open(filename);
    if(!file)
    {
        Serial.println("Could not open file " + filename);
        return "";
    }

    String file_content = "";

    while(file.available())
    {
        file_content = file_content + file.readString();
    }
    
    file.close();

    return file_content;
}