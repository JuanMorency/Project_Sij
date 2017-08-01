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
//desired frequency = 100 Hertz for now
#define ESC_PERIOD 78


// desired frequency = 20 Hertz
// Cannot be too fast as it slows down the rest of the code
// Normally the there should not be continuous interrupts for LED strips 
// And the color should only be changed during an emergency
#define WS2812_PERIOD 393

// 10 Hertz for now
#define SERIAL_PERIOD 784

// 100 Hz
//for some unknown reason this had to be slowed down for the actual frequency to be 100
//#define AK8963_PERIOD 78
#define AK8963_PERIOD 82

// 60 Hz which results in 10 Hz real update frequency
#define BMP180_PERIOD 131

//50 Hz cannot be faster than the rate at which the 4 channel RF is sent, which is around 500 Hz for now
#define RF_TIMEOUT_PERIOD 156

extern volatile bool flagEsc;
extern volatile bool flagWs2812;
extern volatile bool flagSerial;
extern volatile bool flagAk8963;
extern volatile bool flagBmp180;
extern volatile bool flagRfTimeout;

extern volatile unsigned long timer0OverflowCountAk8963, timer0OverflowCountMpu9255, timer0OverflowCountBmp180;
extern float deltaTimeAk8963, deltaTimeMpu9255, deltaTimeBmp180; // time taken for the between polls of sensors
extern uint8_t lastUpdateAk8963, lastUpdateMpu9255, lastUpdateBmp180; // used to calculate integration interval
extern uint8_t Now;        // used to calculate integration interval

extern XYZ16_TypeDef currentRawAcc;
extern XYZ16_TypeDef currentRawGyr;
extern XYZ16_TypeDef currentRawMag;
extern int32_t currentRawTemp;
extern int32_t currentRawPress;

void startInterrupt();
#endif