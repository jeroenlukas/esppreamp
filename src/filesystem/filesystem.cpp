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

    // Read models
    File file_models = LittleFS.open("/models.yaml");
    if(!file_models)
    {
        Serial.println("Could not open file");
        return;
    }

    while(file_models.available())
    {
        Serial.write(file_models.read());
    }
    file_models.close();

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