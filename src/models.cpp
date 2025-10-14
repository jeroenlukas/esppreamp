#include <Arduino.h>
#include <ArduinoJson.h>
#include <YAMLDuino.h>

#include "models.h"
#include "adau1701/adau1701.h"

#include "filesystem/filesystem.h"

#define MIN 0
#define MAX 1

Model_t model_active;

bool models_find(Model_t* dest, uint8_t id);

String model_to_yaml(Model_t model);
String model_to_json(Model_t model);

void model_activate(uint8_t id);
void model_activate(Model_t model);

// -----

String model_to_yaml(Model_t model)
{
    // Use model_to_json, then convert json to yaml
    String json;
    json = model_to_json(model);

    String yaml;
    YAMLNode yamlnode = YAMLNode::loadString(json.c_str());
    serializeYml(yamlnode.getDocument(), yaml, OUTPUT_YAML);
    return yaml;
}


String model_to_json(Model_t model)
{
    JsonDocument doc;

    doc["name"] = model.name;
    doc["id"] = model.id;
    doc["pre"]["cutoff_freq"] = model.pre_cutoff_freq;
    doc["pre"]["order"] = model.pre_order;
    doc["distortion"]["enabled"] = model.dist_enabled;
    doc["distortion"]["gain"][MIN] = model.dist_gain_db.min;
    doc["distortion"]["gain"][MAX] = model.dist_gain_db.max;
    doc["distortion"]["alpha"] = model.dist_alpha;
    doc["distortion"]["asymmetry"] = model.dist_asymmetry;
    doc["distortion"]["volume"] = model.dist_volume;
    doc["tonecontrol"]["low_cutoff_freq"] = model.low_cutoff_freq;
    doc["tonecontrol"]["low_order"] = model.low_order;
    doc["tonecontrol"]["low"][MIN] = model.low_db.min;
    doc["tonecontrol"]["low"][MAX] = model.low_db.max;
    doc["tonecontrol"]["mid"][MIN] = model.mid_db.min;
    doc["tonecontrol"]["mid"][MAX] = model.mid_db.max;
    doc["tonecontrol"]["mid_q"] = model.mid_q;
    doc["tonecontrol"]["mid_freq"] = model.mid_freq;
    doc["tonecontrol"]["high"][MIN] = model.high_db.min;
    doc["tonecontrol"]["high"][MAX] = model.high_db.max;
    doc["presence"]["cutoff_freq"][MIN] = model.presence_cutoff_freq.min;
    doc["presence"]["cutoff_freq"][MAX] = model.presence_cutoff_freq.max;
    doc["presence"]["order"] = model.presence_order;
        
    String json;
    serializeJson(doc, json);   

    return json;
}

/*
bool model_search_yaml(String* dest, uint8_t id)
{
    // Read file contents
    String yaml_models = filesystem_readfile("/models.yaml");

    // Convert yaml to json
    YAMLNode yamlnode = YAMLNode::loadString(yaml_models.c_str());
    String json_str;
    serializeYml(yamlnode.getDocument(), json_str, OUTPUT_JSON_PRETTY);

    // Deserialize json
    JsonDocument doc;
    auto error = deserializeJson(doc, json_str);

    if(error) {
        Serial.printf("Unable to deserialize demo YAML to JsonObject: %s", error.c_str() );
        return false;
    }

    // Parse as an array
    JsonArray array = doc.as<JsonArray>();

    // Search by id
    for(JsonVariant v : array) 
    {
        if(v["id"].as<uint8_t>() == id)
        {            
            String json;
            serializeJson(v, json);

            String yaml;
            YAMLNode yamlnode = YAMLNode::loadString(json.c_str());
            serializeYml(yamlnode.getDocument(), yaml, OUTPUT_YAML);

            *dest = (String)yaml;
            return true;
        }
    }

    return false;
}*/

// Return a Model_t by id
bool models_find(Model_t* dest, uint8_t id)
{
    Model_t model;

    JsonDocument doc;
    String yaml_models = filesystem_readfile("/models.yaml");

    YAMLNode yamlnode = YAMLNode::loadString(yaml_models.c_str());
    String json_str;
    serializeYml(yamlnode.getDocument(), json_str, OUTPUT_JSON_PRETTY);

    //Serial.println("JSON: " + json_str);

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
        return false;
    }

    // Return model 
    *dest = (Model_t)model;
    return true;    
}

// Model-specific settings go here
void model_activate(uint8_t id)
{
    Model_t model;
    models_find(&model, id);
    model_activate(model);
}

void model_activate(Model_t model)
{
    model_active = model;
    Serial.println("Activate model #" + String(model.id) + " (" + model.name + ")");

    float volume_db;

    // Set model-related things
    adau1701_set_pre_cutoff_freq(model_active.pre_cutoff_freq);
    adau1701_set_pre_order(model_active.pre_order);

    adau1701_set_distortion_alpha(model_active.dist_alpha);
    adau1701_set_distortion_asymmetry(model_active.dist_asymmetry);
    adau1701_set_distortion_volume(model_active.dist_volume);

}