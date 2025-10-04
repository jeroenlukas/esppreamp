#include <Arduino.h>
#include <Wire.h>
#include <SigmaDSP.h>

#include "adau1701.h"

#include "..\sigmastudio\SigmaDSP_parameters.h"

SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.0f);

void adau1701_init(void);
void adau1701_set_testfreq(int frequency);
void adau1701_set_gain(float gain);

void adau1701_init(void)
{
    
    Serial.println("Init ADAU1701");
    //
    dsp.begin();

    Serial.println(F("Pinging i2c lines...\n0 -> deveice is present\n2 -> device is not present"));
    Serial.print(F("DSP response: "));
    Serial.println(dsp.ping());
    //Serial.print(F("EEPROM ping: "));
    //Serial.println(ee.ping());
    
    
    // Use this step if no EEPROM is present
    Serial.print(F("\nLoading DSP program... "));
    loadProgram(dsp);
    Serial.println("Done!\n");  
}

void adau1701_set_gain(float gain)
{
    Serial.println("Set gain to " + String(gain));
    //dsp.volume(MOD_DSPDISTORTION_VOLUME_GAIN1940ALGNS4_ADDR, gain);
    // enable dist
    dsp.safeload_write(MOD_DSPDISTORTION_BYPASS_ALG0_MONOSWSLEW_ADDR, 0);
    dsp.gain(MOD_DSPDISTORTION_GAIN1_GAIN1940ALGNS3_ADDR, gain);
    //dsp.volume_slew(MOD_PO_VOLUME_ALG0_TARGET_ADDR, gain, 12);
    
    
}

void adau1701_set_testfreq(int frequency)
{
    
}

// 