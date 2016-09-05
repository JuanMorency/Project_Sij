#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


int main()
{
	DDRH = 0xFF;
	for(;;)
	{
		PORTH = 0xFF;
		_delay_ms(1000);
		PORTH = 0x00;
		_delay_ms(1000);
	}
	return 0;
}