#include <Arduino.h>
#include <ArduinoJson.h>
#include <YAMLDuino.h>

#include "models.h"

#include "filesystem/filesystem.h"

#define MIN 0
#define MAX 1

// Return a model by id
bool models_load(uint8_t id, Model_t* dest)
{
    Model_t model;

    JsonDocument doc;
    String yaml_models = filesystem_readfile("/models.yaml");

    YAMLNode yamlnode = YAMLNode::loadString(yaml_models.c_str());
    String json_str;
    serializeYml(yamlnode.getDocument(), json_str, OUTPUT_JSON_PRETTY);

    Serial.println("JSON: " + json_str);

    auto error = deserializeJson(doc, json_str);

    if(error) {
        Serial.printf("Unable to deserialize demo YAML to JsonObject: %s", error.c_str() );
        return false;
    }

    JsonArray array = doc.as<JsonArray>();

    bool found = false;

    // Iterate through the array of models
    for(JsonVariant v : array) 
    {
        if(v["id"].as<uint8_t>() == id)
        {            
            // Model found, copy values to Model_t object
            found = true;
            model.name = v["name"].as<String>();
            model.id = id;

            // Pre-dist
            model.pre_cutoff_freq = v["pre"]["cutoff_freq"];
            model.pre_order = v["pre"]["order"];

            // Distortion
            model.dist_enabled = v["distortion"]["enabled"];
            model.dist_gain_db.min = v["distortion"]["gain"][MIN];
            model.dist_gain_db.max = v["distortion"]["gain"][MAX];
            model.dist_alpha = v["distortion"]["alpha"];
            model.dist_asymmetry = v["distortion"]["asymmetry"];
            model.dist_volume = v["distortion"]["volume"];
            
            // Tonecontrol
            model.low_cutoff_freq = v["tonecontrol"]["low_cutoff_freq"];
            model.low_order = v["tonecontrol"]["low_order"];
            model.low_db.min = v["tonecontrol"]["low"][MIN];
            model.low_db.max = v["tonecontrol"]["low"][MAX];
            model.mid_db.min = v["tonecontrol"]["mid"][MIN];
            model.mid_db.max = v["tonecontrol"]["mid"][MAX];
            model.mid_q = v["tonecontrol"]["mid_q"];
            model.mid_freq = v["tonecontrol"]["mid_freq"];
            model.high_db.min = v["tonecontrol"]["high"][MIN];
            model.high_db.max = v["tonecontrol"]["high"][MAX];

            // Presence
            model.presence_cutoff_freq.min = v["presence"]["cutoff_freq"][MIN];
            model.presence_cutoff_freq.max = v["presence"]["cutoff_freq"][MAX];
            model.presence_order = v["presence"]["order"];   
        }
    }

    if(!found)
    {
        Serial.println("Could not find model!");
        return false;
    }

    // Return model 
    *dest = (Model_t)model;
    return true;    
}
