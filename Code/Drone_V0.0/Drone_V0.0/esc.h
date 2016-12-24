#ifndef ESC_H
#define ESC_H
#define F_CPU 16000000
#define DELAY_ESC 0 //time to initialize the ESC in microseconds
#define BL 1
#define FL 2
#define FR 3
#define BR 4

#include <avr/io.h>
#include <util/delay.h>

class Esc
{
	int escNumber;
public:
	Esc(int escPosition);
	void set(uint16_t pwm);	
};

	void initializeESC();
#endif
