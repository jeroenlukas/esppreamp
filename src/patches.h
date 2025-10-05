#ifndef PATCHES_H
#define PATCHES_H



class Patch
{
    public:
        Patch();

        String name;
        uint8_t index;
        
        uint8_t model_index;
        
        uint8_t gain;
        uint8_t low;
        uint8_t mid;
        uint8_t high;
        uint8_t presence;
        uint8_t volume;
        
    private:
};

Patch::Patch()
{

}

extern Patch patch_active;

#endif