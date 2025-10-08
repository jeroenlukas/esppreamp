#include <Arduino.h>
#include <ArduinoJson.h>
#include <YAMLDuino.h>

#include "patches.h"
#include "filesystem/filesystem.h"

Patch_t patch_active;

bool patches_find(Patch_t* dest, uint8_t id);
String patch_to_json(Patch_t patch);
String patch_to_yaml(Patch_t patch);

String patch_to_yaml(Patch_t patch)
{
    // Use patch_to_json, then convert json to yaml
    String json;
    json = patch_to_json(patch);

    String yaml;
    YAMLNode yamlnode = YAMLNode::loadString(json.c_str());
    serializeYml(yamlnode.getDocument(), yaml, OUTPUT_YAML);
    return yaml;
}

String patch_to_json(Patch_t patch)
{
    JsonDocument doc;

    doc["name"] = patch.name;
    doc["id"] = patch.id;
    doc["model_id"] = patch.model_id;
    doc["gain"] = patch.gain;
    doc["low"] = patch.low;
    doc["mid"] = patch.mid;
    doc["high"] = patch.high;
    doc["presence"] = patch.presence;
    doc["volume"] = patch.volume;
    
    String json;
    serializeJson(doc, json);   

    return json;
}

bool patches_find(Patch_t* dest, uint8_t id)
{
    Patch_t patch;

    JsonDocument doc;
    String yaml_patches = filesystem_readfile("/patches.yaml");

    YAMLNode yamlnode = YAMLNode::loadString(yaml_patches.c_str());
    String json_str;
    serializeYml(yamlnode.getDocument(), json_str, OUTPUT_JSON_PRETTY);

    auto error = deserializeJson(doc, json_str);

    if(error) {
        Serial.printf("Unable to deserialize demo YAML to JsonObject: %s", error.c_str() );
        return false;
    }

    JsonArray array = doc.as<JsonArray>();

    bool found = false;

    // Iterate through the array of patches
    for(JsonVariant v : array) 
    {
        if(v["id"].as<uint8_t>() == id)
        {            
            // Model found, copy values to Model_t object
            found = true;
            patch.name = v["name"].as<String>();
            patch.id = id;
            patch.model_id = v["model_id"];
            patch.gain = v["gain"];
            patch.low = v["low"];
            patch.mid = v["mid"];
            patch.high = v["high"];
            patch.presence = v["presence"];
            patch.volume = v["volume"];           
        }
    }

    if(!found)
    {        
        return false;
    }

    // Return model 
    *dest = (Patch_t)patch;
    return true;    
}