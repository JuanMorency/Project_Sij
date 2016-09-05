#include "esc.h"

/*
esc 1 = OC4A
esc 2 = OC4B
esc 3 = 0C5A
esc 4 = 0C5B
*/
esc::esc()
{

}

void esc::initialize()
{
  //Timer/Counter 4/5 set to inverted Phase Correct PWM (8 bits resolution)
  TCCR4A |= (1 << COM4A1) | (1 << COM4B1) | (1 << WGM40);
  TCCR5A |= (1 << COM5A1) | (1 << COM5B1) | (1 << WGM50);

  //Prescaler
  TCCR4B |= 0; //à changer!!!!!! vérifier prescaler
  TCCR5B |= 0; //même chose

  //Force Output Compare disabled
  TCCR4C = 0;
  TCCR5C = 0;

  //Timer 4/5 set to 0
  TCNT4 = 0;
  TCNT5 = 0;

  //Set ESCs speed to 0
  OCR4A = 0;
  OCR4B = 0;
  OCR5A = 0;
  OCR5B = 0;
}
