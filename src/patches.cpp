#include <Arduino.h>

#include "patches.h"

Patch patch_active;

void patches_init(void)
{
    
}

// Load patch settings into the active patch
void patches_load(uint8_t index)
{
    patch_active.index = index;
    patch_active.name = "Clean";

    patch_active.model_index = 1;

    patch_active.gain = 20;
    patch_active.low = 50;
    patch_active.mid = 20;
    patch_active.high = 70;
    
    patch_active.presence = 80;
    patch_active.volume = 70;
}

void patch_activate(void)
{

}