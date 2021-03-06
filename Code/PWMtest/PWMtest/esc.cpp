#include "esc.h"


/*
Pour l'instant Fclk_I/O = 1M
la fréquence du PWM est égale à Fclk_IO/(prescaler*2*TOP)
fréquence désiré = entre 100 et 500Hz
Comme on veut un maximum de résolution (le plus grand top possible)
on maximise top et on laisse prescaler à 1
pour un Top à 4096:  1 000 000/(4096*2) = 122Hz
1/122Hz = 8.192ms 
alors si on veut entre 1 et 2 ms, il faut un signal entre 
512 et 1024 pour OCRnX

Lorsqu'on aura set le crystal Fclk_I/O = 16M
donc on aura qu'à changer Top pour le metre à 65536
pour avoir la même fréquence mais encore plus de résolution
La valeur correspondant à 1 et 2 ms sera alors:
8192 et 16384 respectivement

Régistres (p. 154)


COMnA1 = 1 COMnA0 = 0 pour un non inverting mode
en montant, OCR, on va monter le duty cycle

WGMnx3:0 = 1010 (p. 145)
WGMnx1 = 1 and WGMnx3 = 1
On utilise le mode 10 pour avoir un phase correct PWM
avec le plus de résolution possible

ICRnxH and ICRnxL = 0x0FFF
on mets cette valeur à 4096 alors
ICRnH = 00001111 and ICRnL = 11111111

CSn2:0 = 001
prescaler mettre à 1


set pin direction to output (DDR_OCnx)


esc 1 = OC4A
esc 2 = OC4B
esc 3 = 0C5A
esc 4 = 0C5B
*/

//constructor
esc::esc()
{

}

void esc::initialize()
{
  //Timer/Counter 4/5 set to non-inverted Phase Correct PWM (8 bits resolution)
  TCCR4A |= (1 << COM4A1) | (1 << COM4B1) | (1 << WGM41);
  TCCR5A |= (1 << COM5A1) | (1 << COM5B1) | (1 << WGM51);

  //wave generator mode à phase correct full resolution et Prescaler = 1

  TCCR4B |= (1 << WGM43) | (1 << CS40) ;
  TCCR5B |= (1 << WGM53) | (1 << CS50) ;
  
  //régler TOP à 4096
  ICR4 = 4096;
  ICR5 = 4096;
  
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

void esc::set(uint16_t pwm)
{
	OCR4B = pwm;
}