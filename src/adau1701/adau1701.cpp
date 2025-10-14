#include <Arduino.h>
#include <Wire.h>
#include <SigmaDSP.h>

#include "adau1701.h"

#include "..\defines.h"

#include "..\sigmastudio\SigmaDSP_parameters.h"

SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.0f);

void adau1701_init(void);
void adau1701_set_testfreq(int frequency);
void adau1701_set_gain(float gain);
void adau1701_set_high(float high);
void adau1701_set_low(float low);
void adau1701_set_tonecontrol(float low, float high, float center_freq);
void adau1701_set_volume(float volume_db);

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

    // Reset
    dsp.reset();
    
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

void adau1701_set_mid(float mid_db, float center_freq, float q)
{
    // Mid cut is divided between two -10..+10dB filters.
    // Also give it a slight offset so there is a default mid cut.
    mid_db = mid_db / 2;
    mid_db -= 3;

    secondOrderEQ_t eq;

    eq.filterType = parameters::filterType::peaking;
    eq.boost = mid_db;
    eq.Q = q;
    eq.freq = center_freq;
    
    Serial.println("[ADAU1701] set mid to " + String(mid_db) + " db, freq=" + String(center_freq) + " Hz, Q=" + String(q));
    dsp.EQsecondOrder(MOD_POSTGAIN_MID1_ALG0_STAGE0_B0_ADDR, eq);
    dsp.EQsecondOrder(MOD_POSTGAIN_MID2_ALG0_STAGE0_B0_ADDR, eq);
}

void adau1701_set_volume(float volume_dB)
{
    dsp.gain(MOD_GAIN1_2_GAIN1940ALGNS7_ADDR, 1.0);
    dsp.volume_slew(MOD_PO_VOLUME_ALG0_TARGET_ADDR, volume_dB);
}

void adau1701_set_distortion_volume(float volume_db)
{
    float volume_fac = pow(10, volume_db / 20);
    Serial.println("Set distortion_volume to " + String(volume_db) + " -> " + String(volume_fac ));
    dsp.volume_slew(MOD_DSPDISTORTION_VOLUME_GAIN1940ALGNS4_ADDR, volume_fac);
}

void adau1701_set_pre_cutoff_freq(float freq)
{
    Serial.println("Set pre cutoff to " + String(freq) + " Hz");
    firstOrderEQ_t eq;
    eq.filterType = parameters::filterType::highpass;
    eq.freq = freq;
    eq.gain = 0;
    
    dsp.EQfirstOrder(MOD_PREGAIN_LOWCUT_ALG0_PARAMB00_ADDR, eq);
    dsp.EQfirstOrder(MOD_PREGAIN_LOWCUT2_ALG0_PARAMB00_ADDR, eq);
    dsp.EQfirstOrder(MOD_PREGAIN_LOWCUT3_ALG0_PARAMB00_ADDR, eq);
    dsp.EQfirstOrder(MOD_PREGAIN_LOWCUT4_ALG0_PARAMB00_ADDR, eq);    
}

void adau1701_set_pre_order(uint8_t order)
{
    // TODO
    Serial.println("Try to set pre order to " + String(order));
    if((order > 0) && (order < 5))
    {
        Serial.println("Set Pre order to " + String(order));
        uint8_t sw = order - 1;
        Serial.println("sw: " + String(sw));
        dsp.mux(MOD_PREGAIN_ORDER_ALG0_MONOSWSLEW_ADDR, sw, 0);
    }
}

void adau1701_set_distortion_alpha(float alpha)
{
    dsp.softClip(MOD_DSPDISTORTION_SOFTCLIP1_ALG0_SOFTCLIPALGG21ALPHA_ADDR, alpha);
}

void adau1701_set_distortion_asymmetry(float asymmetry)
{
    dsp.dcSource(MOD_DSPDISTORTION_ASYMMETRY_DCINPALG1_ADDR, asymmetry);
}

void adau1701_set_bypass(bool bypass, uint8_t block)
{
    // Blocks:
    // 0: all (not implemented)
    // 1: Pregain
    // 2: distortion
    // 3: postgain
    // 4: presence (not implemented in sigma circuit yet)

    // Demux value: 1=bypass, 0=active
    uint8_t bypass_idx = 0;
    if(bypass)
        bypass_idx = 1;

    // TODO:
    //rework sigma studio project. only use slew mux and set correct numberofindexes
    // Only use noslew switches! slew mux dont work?
    //https://github.com/jeroenlukas/dsppreamp/blob/7cad7f19ddbc31a3cb6942c825b9818f49f0ff0d/models.c#L124

    switch(block)
    {
        case BYPASS_ALL:
            // Todo            
            break;
        case BYPASS_PREGAIN:
            dsp.mux(MOD_PREGAIN_BYPASS_ALG0_STAGE0_MONOSWITCHNOSLEW_ADDR, bypass_idx, 2);
            break;
        case BYPASS_DISTORTION:
            dsp.mux(MOD_DSPDISTORTION_BYPASS_ALG0_STAGE0_MONOSWITCHNOSLEW_ADDR, bypass_idx, 2);
            break;
        case BYPASS_POSTGAIN:
            dsp.mux(MOD_POSTGAIN_PO_BYPASS_ALG0_STAGE0_MONOSWITCHNOSLEW_ADDR, bypass_idx, 2);
            break;
        case BYPASS_PRESENCE:
            // Todo
            break;
        default:
            Serial.println("[ERROR] Invalid value " + String(block));

    }
}

float adau1701_read_signal_in(void)
{
    //dsp.readBack()
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