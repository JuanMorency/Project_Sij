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
#define CHANNEL_1_RANGE (CHANNEL_1_MAX_PWM-CHANNEL_1_MIN_PWM)
#define CHANNEL_2_RANGE (CHANNEL_2_MAX_PWM-CHANNEL_2_MIN_PWM)
#define CHANNEL_3_RANGE (CHANNEL_3_MAX_PWM-CHANNEL_3_MIN_PWM)
#define CHANNEL_4_RANGE (CHANNEL_4_MAX_PWM-CHANNEL_4_MIN_PWM)
#define CHANNEL_1_MEAN (CHANNEL_1_MAX_PWM+CHANNEL_1_MIN_PWM)/2
#define CHANNEL_2_MEAN (CHANNEL_2_MAX_PWM+CHANNEL_2_MIN_PWM)/2
#define CHANNEL_3_MEAN (CHANNEL_3_MAX_PWM+CHANNEL_3_MIN_PWM)/2
#define CHANNEL_4_MEAN (CHANNEL_4_MAX_PWM+CHANNEL_4_MIN_PWM)/2


#define MAX_ROLL_CONTROL 60 //in degrees
#define MAX_PITCH_CONTROL 60 //in degrees
#define MAX_YAW_CONTROL 120 //in degrees

#define YAW 0
#define PITCH 1
#define ROLL 2

extern uint16_t ch_1_pw, ch_2_pw, ch_3_pw, ch_4_pw;
extern bool RFInitialized;
extern uint8_t channel1OverflowCount, channel2OverflowCount, channel3OverflowCount, channel4OverflowCount;

void initRF();
void handleFSMRF(void);
float getDesiredAngleFromRf(uint8_t desiredAngle);

#endif