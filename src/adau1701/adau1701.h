#ifndef EP_ADAU1701_H
#define EP_ADAU1701_H

extern void adau1701_init(void);
extern void adau1701_set_testfreq(int frequency);
extern void adau1701_set_gain(float gain);
extern void adau1701_set_high(float high);
extern void adau1701_set_low(float low);
extern void adau1701_set_tonecontrol(float low, float high, float center_freq);
extern void adau1701_set_mid(float mid_db, float center_freq, float q);

#endif