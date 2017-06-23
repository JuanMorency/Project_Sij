#ifndef INTERRUPT_H
#define INTERRUPT_H

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

//to get the initialization status
#include "lcd.h" 
#include "esc.h" 
#include "WS2812.h"
#include "IMU.h"
#include "RF.h"
#include "serial.h"

// xxx_Period defines the frequency at which the xxx interrupt flag is set to 1
// Use the following formula to determine xxx_Period:
// xxx_Period = F_CPU/OCR0A/prescaler/Desired_frequency_of_interrupts
// F_CPU = 16M, OCR0A = 255, Prescaler = 8, 


// The minimum update frequency for multirotors is generally 200 Hz
// https://www.rcgroups.com/forums/showthread.php?1504894-ESC-update-frequency
//desired frequency = 200 Hertz
#define ESC_PERIOD 39


// desired frequency = 20 Hertz
// Cannot be too fast as it slows down the rest of the code
// Normally the there should not be continuous interrupts for LED strips 
// And the color should only be changed during an emergency
#define WS2812_PERIOD 393

// 60 Hertz for now
#define IMU_PERIOD 1

extern volatile bool flagLCD;
extern volatile bool flagRF;
extern volatile bool flagESC;
extern volatile bool flagWS2812;
extern volatile bool flagIMU;

extern volatile unsigned long timer0_overflow_count;
extern float deltat;     // integration interval for both filter schemes
extern uint8_t lastUpdate;	// used to calculate integration interval for IMU
extern uint8_t Now;			// used to calculate integration interval for IMU

void startInterrupt();
#endif