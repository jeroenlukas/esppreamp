#include <Arduino.h>
#include <Wire.h>
#include <SigmaDSP.h>

#include "adau1701.h"

#include "..\sigmastudio\SigmaDSP_parameters.h"

SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.0f);

void adau1701_init(void);
void adau1701_set_testfreq(int frequency);
void adau1701_set_gain(float gain);
void adau1701_set_high(float high);
void adau1701_set_low(float low);
void adau1701_set_tonecontrol(float low, float high, float center_freq);

void adau1701_init(void)
{
    
    Serial.println("Init ADAU1701");
    //
    dsp.begin();

    Serial.println(F("Pinging ADAU1701..."));
    if(dsp.ping() == 0)
    {
        Serial.println("OK");
    }
    else Serial.println("Could not find DSP!");
    
    Serial.print(F("Loading DSP algo..."));

    loadProgram(dsp);

    Serial.println("Done");  
}

void adau1701_set_gain(float gain_db)
{
    if(gain_db > 72.0)
    {
        gain_db = 72.0;
    }
    Serial.println("Set gain to " + String(gain_db));
    //dsp.volume(MOD_DSPDISTORTION_VOLUME_GAIN1940ALGNS4_ADDR, gain);
    // enable dist
    // Actually the total gain of 72 dB is spread accross 3 gain stages with 24 dB max gain
    double gain_divided = pow(10, gain_db/3 / 20);

    dsp.safeload_write(MOD_DSPDISTORTION_BYPASS_ALG0_MONOSWSLEW_ADDR, 0);
    dsp.gain(MOD_DSPDISTORTION_GAIN1_GAIN1940ALGNS3_ADDR, gain_divided);
    dsp.gain(MOD_DSPDISTORTION_GAIN2_GAIN1940ALGNS5_ADDR, gain_divided);
    dsp.gain(MOD_DSPDISTORTION_GAIN3_GAIN1940ALGNS6_ADDR, gain_divided);
    //dsp.volume_slew(MOD_PO_VOLUME_ALG0_TARGET_ADDR, gain, 12);
    
    
}

void adau1701_set_tonecontrol(float low, float high, float center_freq)
{
    //Serial.println("Set high to " + String(high));
    toneCtrl_t tone_ctl;
    tone_ctl.boost_Bass_dB = low;
    tone_ctl.boost_Treble_dB = high;
    tone_ctl.freq_Bass = center_freq;
    tone_ctl.freq_Treble = center_freq;
    dsp.toneControl(MOD_POSTGAIN_PO_TONECONTROL_ALG0_STAGE0_B0_ADDR, tone_ctl);
}

// todo add q?
void adau1701_set_mid(float mid_db, float center_freq, float q)
{
    secondOrderEQ_t eq;

    eq.filterType = parameters::filterType::peaking;
    eq.boost = mid_db;
    eq.Q = q;
    eq.freq = center_freq;

    Serial.println("[ADAU1701] set mid to " + String(mid_db) + " db, freq=" + String(center_freq) + " Hz, Q=" + String(q));
    dsp.EQsecondOrder(MOD_POSTGAIN_MID1_ALG0_STAGE0_B0_ADDR, eq);
    dsp.EQsecondOrder(MOD_POSTGAIN_MID2_ALG0_STAGE0_B0_ADDR, eq);
}

/*
void adau1701_set_high(float high)
{
    Serial.println("Set high to " + String(high));
    toneCtrl_t tone_ctl;
    tone_ctl.boost_Bass_dB = 2;
    tone_ctl.boost_Treble_dB = high;
    tone_ctl.freq_Bass = 400;
    tone_ctl.freq_Treble = 400;
    dsp.toneControl(MOD_POSTGAIN_PO_TONECONTROL_ALG0_STAGE0_B0_ADDR, tone_ctl);
}

void adau1701_set_low(float low)
{
    Serial.println("Set low to " + String(low));
    toneCtrl_t tone_ctl;
    tone_ctl.boost_Bass_dB = low;
    tone_ctl.boost_Treble_dB = 2;
    tone_ctl.freq_Bass = 400;
    tone_ctl.freq_Treble = 400;
    dsp.toneControl(MOD_POSTGAIN_PO_TONECONTROL_ALG0_STAGE0_B0_ADDR, tone_ctl);
}
*/
void adau1701_set_testfreq(int frequency)
{
    
}

// 