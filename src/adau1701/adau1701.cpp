#include <Arduino.h>
#include <Wire.h>
#include <SigmaDSP.h>

#include "adau1701.h"

#include "..\sigmastudio\SigmaDSP_parameters.h"

SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.0f);

void adau1701_init(void);
void adau1701_set_testfreq(int frequency);

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

void adau1701_set_testfreq(int frequency)
{
    dsp.sawtoothSource(MOD_SAWTOOTH2_ALG0_FREQ_ADDR, frequency);
}

// 