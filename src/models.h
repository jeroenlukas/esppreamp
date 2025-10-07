#ifndef MODELS_H
#define MODELS_H

struct MinMax_float_t
{
    float min;
    float max;
} ;

class Model_t
{
    public:
        //Model();

        String name;
        uint8_t id;

        // Pre-distortion
        float pre_cutoff_freq;
        uint8_t pre_order;

        // Distortion 
        bool dist_enabled;
        MinMax_float_t dist_gain_db;
        float dist_alpha;
        float dist_asymmetry;
        float dist_volume;

        // Tonecontrol
        float low_cutoff_freq;
        uint8_t low_order;
        MinMax_float_t low_db;
        MinMax_float_t mid_db;
        float mid_q;
        float mid_freq;
        MinMax_float_t high_db;

        // Presence
        MinMax_float_t presence_cutoff_freq;
        uint8_t presence_order;
};



extern bool models_load(uint8_t id, Model_t* dest);
extern bool model_search_yaml(String* dest, uint8_t id);

#endif