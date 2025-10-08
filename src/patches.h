#ifndef PATCHES_H
#define PATCHES_H



class Patch_t
{
    public:
        String name;
        uint8_t id;
        
        uint8_t model_id;
        
        uint8_t gain;
        uint8_t low;
        uint8_t mid;
        uint8_t high;
        uint8_t presence;
        uint8_t volume;
        
    private:
};

extern Patch_t patch_active;

extern bool patches_find(Patch_t* dest, uint8_t id);
extern String patch_to_json(Patch_t patch);
extern String patch_to_yaml(Patch_t patch);

#endif