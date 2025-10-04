/*
 * File:           C:\Users\Jeroen\Jottacloud\DIY\Elektronica\ESPPreamp\Software\ESPPreamp\src\sigmastudio\Design 1_IC_1_PARAM.h
 *
 * Created:        Saturday, October 4, 2025 1:57:12 PM
 * Description:    Design 1:IC 1 parameter RAM definitions.
 *
 * This software is distributed in the hope that it will be useful,
 * but is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This software may only be used to program products purchased from
 * Analog Devices for incorporation by you into audio products that
 * are intended for resale to audio product end users. This software
 * may not be distributed whole or in any part to third parties.
 *
 * Copyright ©2025 Analog Devices, Inc. All rights reserved.
 */
#ifndef __DESIGN_1_IC_1_PARAM_H__
#define __DESIGN_1_IC_1_PARAM_H__


/* Module Sawtooth1 - Sawtooth wave*/
#define MOD_SAWTOOTH1_COUNT                            2
#define MOD_SAWTOOTH1_DEVICE                           "IC1"
#define MOD_SAWTOOTH1_ALG0_FREQ_ADDR                   1
#define MOD_SAWTOOTH1_ALG0_FREQ_FIXPT                  0x00015557
#define MOD_SAWTOOTH1_ALG0_FREQ_VALUE                  SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.010416875)
#define MOD_SAWTOOTH1_ALG0_FREQ_TYPE                   SIGMASTUDIOTYPE_FIXPOINT
#define MOD_SAWTOOTH1_ALG0_ON_ADDR                     2
#define MOD_SAWTOOTH1_ALG0_ON_FIXPT                    0x00800000
#define MOD_SAWTOOTH1_ALG0_ON_VALUE                    SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_SAWTOOTH1_ALG0_ON_TYPE                     SIGMASTUDIOTYPE_FIXPOINT

/* Module Sawtooth2 - Sawtooth wave*/
#define MOD_SAWTOOTH2_COUNT                            2
#define MOD_SAWTOOTH2_DEVICE                           "IC1"
#define MOD_SAWTOOTH2_ALG0_FREQ_ADDR                   4
#define MOD_SAWTOOTH2_ALG0_FREQ_FIXPT                  0x0001DDDD
#define MOD_SAWTOOTH2_ALG0_FREQ_VALUE                  SIGMASTUDIOTYPE_FIXPOINT_CONVERT(0.0145833333333333)
#define MOD_SAWTOOTH2_ALG0_FREQ_TYPE                   SIGMASTUDIOTYPE_FIXPOINT
#define MOD_SAWTOOTH2_ALG0_ON_ADDR                     5
#define MOD_SAWTOOTH2_ALG0_ON_FIXPT                    0x00800000
#define MOD_SAWTOOTH2_ALG0_ON_VALUE                    SIGMASTUDIOTYPE_FIXPOINT_CONVERT(1)
#define MOD_SAWTOOTH2_ALG0_ON_TYPE                     SIGMASTUDIOTYPE_FIXPOINT

#endif
