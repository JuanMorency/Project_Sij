#ifndef ESC_H
#define ESC_H
#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>

class esc
{
public:
  esc();
  void initialize();
};
#endif
