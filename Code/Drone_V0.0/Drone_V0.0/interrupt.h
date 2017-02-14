#ifndef INTERRUPT_H
#define INTERRUPT_H
#define F_CPU 16000000


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

// xxx_Period defines the frequency at which the xxx interrupt flag is set to 1
// Use the following formula to determine xxx_Period:
// xxx_Period = F_CPU/OCR0A/prescaler/Desired_frequency_of_interrupts
// example for LCD : F_CPU = 16M, OCR0A = 255, Prescaler = 1, 
// LCD desired frequency = 20k: LCD_Period = 3

//max 65535 for the periods since this is a 2 bit int
#define LCD_PERIOD 3


//desired frequency = 60 Hertz
#define ESC_PERIOD 1000


// desired frequency = 20 Hertz
// Cannot be too fast as it slows down the rest of the code
// Normally the there should not be continuous interrupts for LED strips 
// And the color should only be changed during an emergency
#define WS2812_PERIOD 3000

// 30 Hertz for now
#define IMU_PERIOD 2000

extern bool flagLCD;
extern bool flagRF;
extern bool flagESC;
extern bool flagWS2812;
extern bool flagIMU;
void startInterrupt();
#endif