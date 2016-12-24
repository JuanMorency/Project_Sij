#include "esc.h"


/*
la fréquence du PWM est égale à Fclk_IO/(prescaler*2*TOP)
fréquence désiré = entre 100 et 500Hz
Comme on veut un maximum de résolution (le plus grand top possible)
on maximise top et on laisse prescaler à 1
pour un Top à 65536:  16 000 000/(65536*2) = 122Hz
1/122Hz = 8.192ms
alors si on veut entre 1 et 2 ms (requis par le ESC), il faut un 
signal entre 8192 et 16384 pour OCRnX

Régistres (p. 154)


COMnA1 = 1 COMnA0 = 0 pour un non inverting mode
en montant, OCR, on va monter le duty cycle

WGMnx3:0 = 1010 (p. 145)
WGMnx1 = 1 and WGMnx3 = 1
On utilise le mode 10 pour avoir un phase correct PWM
avec le plus de résolution possible

ICRnxH and ICRnxL = 0x0FFF
on mets cette valeur à 65535 alors

CSn2:0 = 001
prescaler mettre à 1

set pin direction to output (DDR_OCnx)

esc BL = OC4A
esc FL = OC4B
esc FR = 0C5A
esc BR = 0C5B
*/

//constructor

Esc::Esc(int escPosition)
{
	escNumber = escPosition;
}

void initializeESC()
{
  //Timer/Counter 4/5 set to non-inverted Phase Correct PWM (8 bits resolution)
  TCCR4A |= (1 << COM4A1) | (1 << COM4B1) | (1 << WGM41);
  TCCR5A |= (1 << COM5A1) | (1 << COM5B1) | (1 << WGM51);

  //wave generator mode à phase correct full resolution et Prescaler = 1

  TCCR4B |= (1 << WGM43) | (1 << CS40) ;
  TCCR5B |= (1 << WGM53) | (1 << CS50) ;
  
  //régler TOP à 65535
  ICR4 = 65535;
  ICR5 = 65535;
  
  //Set ESCs speed to 0
  //TODO need to select right initial pulse width such that the ESC get armed
  OCR4A = 0;
  OCR4B = 0;
  OCR5A = 0;
  OCR5B = 0;

  //Set Pins to output
  DDRL |= (1<<PL4) | (1<<PL3); 
  DDRH |= (1<<PH4) | (1<<PH3);  
  
  //delay to make sure the ESC are armed before playing with the PWM
  _delay_ms(DELAY_ESC);
  
  //Timer 4/5 set to 0
  TCNT4 = 0;
  TCNT5 = 0;
}

void Esc::set(uint16_t pwm)
{
	switch (escNumber) {
		//multiply by 4 here to get from the PWM of the RF controller to the ESC
		//ESC requires 1-2ms pulse width so from 8000 to 16000 pwm
		//since the number we get from the RF is between 2000 and 4000, multiply by 4
		case 1: 
			OCR4A = pwm*4;
			break;
		case 2: 
			OCR4B = pwm*4;
			break;
		case 3: 
			OCR5A = pwm*4;
			break;
		case 4: 
			OCR5B = pwm*4;
			break;
		default: 
			OCR4A = 0;
			OCR4B = 0;
			OCR5A = 0;
			OCR5B = 0;
	}
}