#include "interrupt.h"

int button_rising = 0;
int button_falling = 0;

void initializeInterrupt()
{
	PCICR |= (1<<PCIE0); /* enable pin change interrupt 0 */
	PCMSK0 |= (1<<PCINT4); /* enable pin B0 to trigger PCINT0 */
		
	// Turn interrupts on.
	sei();
}

ISR(PCINT0_vect) {
	if (PINB & 0b00010000) button_rising = 1;
	else button_falling = 1;
	//_delay_ms(500); //debounce
}