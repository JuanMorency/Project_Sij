/*
 * Floating_point_test.cpp
 *
 * Created: 6/6/2017 12:12:47
 * Author : juan-
 */ 

#include <avr/io.h>
//#include "MadgwickAHRS.h"



int main(void)
{
	float gx, gy, gz, ax, ay, az, mx, my, mz;
	gx = 0;
	gy = 0;
	gz = 0;
	ax = 200;
	ay = 100;
	az = 10000;
	mx = 0;
	my = 550;
	mz = 0;
	
	float temp = 0;
    /* Replace with your application code */
    while (1) 
    {
		temp = ay+ax;
		temp = ay-ax;
		temp = ay*ax;
		temp = ay/ax;
    }
	
		temp = ay+ax;
		temp = ay-ax;
		temp = ay*ax;
		temp = ay/ax;
}

