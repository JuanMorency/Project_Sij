#ifndef INTERRUPT_H
#define INTERRUPT_H
#define F_CPU 1000000

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

extern int button_rising;
extern int button_falling;
void initializeInterrupt();

#endif