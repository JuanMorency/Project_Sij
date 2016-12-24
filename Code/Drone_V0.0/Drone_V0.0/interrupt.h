#ifndef INTERRUPT_H
#define INTERRUPT_H
#define F_CPU 16000000


#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include "lcd.h" //temporary for testing

extern bool button_rising;
extern bool button_falling;
extern uint16_t ch_1_pw, ch_2_pw, ch_3_pw, ch_4_pw;
void initializeInterrupt();
void initializecounterPWMread();

#endif