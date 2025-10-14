#ifndef EP_ADAU1701_H
#define EP_ADAU1701_H

extern void adau1701_init(void);
extern void adau1701_set_testfreq(int frequency);

// Patch related
extern void adau1701_set_gain(float gain);
extern void adau1701_set_high(float high);
extern void adau1701_set_low(float low);
extern void adau1701_set_tonecontrol(float low, float high, float center_freq);
extern void adau1701_set_mid(float mid_db, float center_freq, float q);
extern void adau1701_set_volume(float volume_db);

// Model related
extern void adau1701_set_distortion_volume(float volume_db);
extern void adau1701_set_pre_cutoff_freq(float freq);
extern void adau1701_set_pre_order(uint8_t order);
extern void adau1701_set_distortion_alpha(float alpha);
extern void adau1701_set_distortion_asymmetry(float asymmetry);

extern void adau1701_set_bypass(bool bypass, uint8_t block);

#endif