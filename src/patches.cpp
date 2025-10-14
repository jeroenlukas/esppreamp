#include <Arduino.h>
#include <ArduinoJson.h>
#include <YAMLDuino.h>

#include "patches.h"

#include "models.h"
#include "adau1701/adau1701.h"
#include "filesystem/filesystem.h"

Patch_t patch_active;

bool patches_find(Patch_t* dest, uint8_t id);
String patch_to_json(Patch_t patch);
String patch_to_yaml(Patch_t patch);

void patch_activate(uint8_t id);
void patch_activate(Patch_t patch);

float map_percent_to_float(float min, float max, uint8_t percentage);

//                               0.2       4.0              60
float map_percent_to_float(float min, float max, uint8_t percentage)
{
    float ret = 0;
    float interm = (max - min) / 100;
    ret = (interm * percentage) + min;

    Serial.println("Mapped result: " + String(ret));
    return ret;
}

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

// Activate patch by id
void patch_activate(uint8_t id)
{
    Patch_t patch;
    patches_find(&patch, id);
    patch_activate(patch);
}

void patch_update_gain(uint8_t perc)
{
    patch_active.gain = perc;
    float gain = map_percent_to_float(model_active.dist_gain_db.min, model_active.dist_gain_db.max, patch_active.gain);

    adau1701_set_gain(gain);
}

void patch_update_low(uint8_t perc)
{
    patch_active.low = perc;
    float low = map_percent_to_float(model_active.low_db.min, model_active.low_db.max, patch_active.low);
    float high = map_percent_to_float(model_active.low_db.min, model_active.low_db.max, patch_active.high); // Unchanged
    adau1701_set_tonecontrol(low, high, model_active.mid_freq);
}

void patch_update_high(uint8_t perc)
{
    patch_active.high = perc;
    float low = map_percent_to_float(model_active.low_db.min, model_active.low_db.max, patch_active.low); // Unchanged
    float high = map_percent_to_float(model_active.high_db.min, model_active.high_db.max, patch_active.high);
    adau1701_set_tonecontrol(low, high, model_active.mid_freq);
}

void patch_update_mid(uint8_t perc)
{
    patch_active.mid = perc;
    float mid = map_percent_to_float(model_active.mid_db.min, model_active.mid_db.max, patch_active.mid);
    adau1701_set_mid(mid, model_active.mid_freq, model_active.mid_q);
}

void patch_update_volume(uint8_t perc)
{
    patch_active.volume = perc;
    float vol = map_percent_to_float(-24, 0, perc);
    adau1701_set_volume(vol);
}

void patch_activate(Patch_t patch)
{
    Serial.println("Activate patch #" + String(patch.id) + " (" + patch.name + ")");
    
    // Make the patch the active patch
    patch_active = patch;

    // Load the model for this patch
    model_activate(patch.model_id);

    // Set patch stuff
    patch_update_gain(patch_active.gain);
    patch_update_low(patch_active.low);
    patch_update_high(patch_active.high);
    patch_update_mid(patch_active.mid);

    patch_update_volume(patch_active.volume);
    // ...


}