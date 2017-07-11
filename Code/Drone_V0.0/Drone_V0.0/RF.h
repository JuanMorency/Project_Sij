#ifndef RF_H
#define RF_H

#include <avr/interrupt.h>
#include "typeDef.h"
#include "filter.h"
#include "debugLED.h"
#include "serial.h"


//Values of our RF receiver when offset control is centered
#define CHANNEL_1_MIN_PWM 2260
#define CHANNEL_2_MIN_PWM 2133
#define CHANNEL_3_MIN_PWM 2200
#define CHANNEL_4_MIN_PWM 2073
#define CHANNEL_1_MAX_PWM 3935
#define CHANNEL_2_MAX_PWM 3758
#define CHANNEL_3_MAX_PWM 3515
#define CHANNEL_4_MAX_PWM 3925

#define RF_MAX_ERROR 200


extern uint16_t ch_1_pw, ch_2_pw, ch_3_pw, ch_4_pw;
extern bool RFInitialized;
extern uint8_t channel1OverflowCount, channel2OverflowCount, channel3OverflowCount, channel4OverflowCount;

void initRF();
void handleFSMRF(void);

#endif