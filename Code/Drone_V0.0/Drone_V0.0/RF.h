#ifndef RF_H
#define RF_H

#include <avr/interrupt.h>
#include "typeDef.h"
#include "filter.h"

extern uint16_t ch_1_pw, ch_2_pw, ch_3_pw, ch_4_pw;
extern bool RFInitialized;

void initRF();
void handleFSMRF(void);

#endif