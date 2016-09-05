#ifndef ESC_H
#define ESC_H
#define F_CPU 1000000
#define DELAY_ESC 0 //time to initialize the ESC in microseconds

#include <avr/io.h>
#include <util/delay.h>

class esc
{
public:
  esc();
  void initialize();
  void set(uint16_t pwm);
};
#endif
