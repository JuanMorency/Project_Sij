/**
  ******************************************************************************
	* File Name         : WS2812.cpp
	* Description       : Class for controlling multiple RGB WS2812 LED strips 
	* Author			: Matthias Riegler, Juan Morency Trudel (Adaptation)
	* Version           : 2.1
	* Date				: September 6, 2014
	* License			: GNU GPL v2 (see License.txt)
	* Comment			: Mar 07 2014: Added Arduino and C++ Library
						  September 6, 2014:	Added option to switch between most popular color orders
						  (RGB, GRB, and BRG) --  Windell H. Oskay
	
  ******************************************************************************
  */

#include "WS2812.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/portpins.h>

bool WS2812Initialized = false;

/**
	* @brief WS2812 class that represents one LED strip
	* @param num_leds	: Number of LED to control on the strip
	* @param stripPas	: Position of the LED strip on the device to assign the right port to control it
	* @retval None
	*/
WS2812::WS2812(uint16_t num_leds, uint8_t stripPos) {
	count_led = num_leds;
	stripPosition = stripPos;
	pixels = (uint8_t*)malloc(count_led*3);
	#ifdef RGB_ORDER_ON_RUNTIME	
		offsetGreen = 0;
		offsetRed = 1;
		offsetBlue = 2;
	#endif
}

/**
	* @brief set the WS2812Initialized to true to use the interrupts associated with this
	* @param None
	* @retval None
	*/
void initWS2812(void){
	WS2812Initialized = true;
}


/**
	* @brief Gets the RGB value of the desired pixel in the array representing the colors of the 
	* LED in the program memory
	* @param index: This is the reference number of the pixel that is desired
	* @retval cRGB: returns the cRGB struct value 
	*/
cRGB WS2812::get_crgb_at(uint16_t index) {
	
	cRGB px_value;
	
	if(index < count_led) {
		
		uint16_t tmp;
		tmp = index * 3;

		px_value.r = pixels[OFFSET_R(tmp)];
		px_value.g = pixels[OFFSET_G(tmp)];
		px_value.b = pixels[OFFSET_B(tmp)];
	}
	
	return px_value;
}


/**
	* @brief Sets the RGB value of the desired pixel in the array representing the colors of the 
	* LED in the program memory
	* @param index: This is the reference number of the pixel that is desired to change
	* @param px_value: This is the color that is to be assigned to the LED
	* @retval uint8_t: returns a 0 upon success and 1 if the the index out of bound
	*/
uint8_t WS2812::set_crgb_at(uint16_t index, cRGB px_value) {
	
	if(index < count_led) {
		
		uint16_t tmp;
		tmp = index * 3;
		
		pixels[OFFSET_R(tmp)] = px_value.r;
		pixels[OFFSET_G(tmp)] = px_value.g;
		pixels[OFFSET_B(tmp)] = px_value.b;		
		return 0;
	} 
	return 1;
}

/**
	* @brief Same as set_crgb_at but with the possibility to input an offset (should be multiple of 3) to
	* facilitate implementation in loops
	* @param index: This is the reference number of the pixel that is desired to change
	* @param offset: The offset of the pixel we want to change. offset of 1 is only one color (RGB), so 
	* if you want to offset by 1 LED, the offset should be a multiple of 3
	* @param px_value: This is the color that is to be assigned to the LED
	* @retval uint8_t: returns a 0 upon success and 1 if the the index out of bound
	*/
uint8_t WS2812::set_subpixel_at(uint16_t index, uint8_t offset, uint8_t px_value) {
	if (index < count_led) {
		uint16_t tmp;
		tmp = index * 3;

		pixels[tmp + offset] = px_value;
		return 0;
	}
	return 1;
}


/**
	* @brief This method sets the right output port associated with the WS2812 object and sends the pixel data to the 
	* LED strip
	* @param none
	* @retval none
	*/
void WS2812::sync() {
	setAsOutput();
	*ws2812_port_reg |= pinMask; // Enable DDR 
	ws2812_sendarray_mask(pixels,3*count_led,pinMask,(uint8_t*) ws2812_port,(uint8_t*) ws2812_port_reg );	
}

#ifdef RGB_ORDER_ON_RUNTIME	
/**
	* @brief Sets the order of the colors to GRB in the WS2812 objects
	* @param none
	* @retval none
	*/
void WS2812::setColorOrderGRB() { // Default color order
	offsetGreen = 0;
	offsetRed = 1;
	offsetBlue = 2;
}
/**
	* @brief Sets the order of the colors to RGB in the WS2812 objects
	* @param none
	* @retval none
	*/
void WS2812::setColorOrderRGB() {
	offsetRed = 0;
	offsetGreen = 1;
	offsetBlue = 2;
}
/**
	* @brief Sets the order of the colors to BRG in the WS2812 objects
	* @param none
	* @retval none
	*/
void WS2812::setColorOrderBRG() {
	offsetBlue = 0;
	offsetRed = 1;
	offsetGreen = 2;
}
#endif

/**
	* @brief Class destructor, restores the memory associated with the pixel array
	* @param none
	* @retval none
	*/
WS2812::~WS2812() {
	free(pixels);
	
}

/**
	* @brief Method for selecting the right output port with the stripPosition attribute in the Object
	* This is based on the mapping done for our drone.
	* This is the place to change the IO pins if we change the pins that control the LED strips
	* @param none
	* @retval none
	*/
void WS2812::setAsOutput()
{
	//seem to have problems with port F, except for F3..., can't do shit
	switch (stripPosition)
	{
	case FRT:
		setOutput(&PORTB,&DDRB, 6);
		break;
	case FRB:
		setOutput(&PORTB,&DDRB, 7);
		break;
	case FLT:
		setOutput(&PORTJ,&DDRJ, 7);
		break;
	case FLB:
		setOutput(&PORTJ,&DDRJ, 6);
		break;
	case BRT:
		setOutput(&PORTF,&DDRF, 7);
		break;
	case BRB:
		setOutput(&PORTF,&DDRF, 6);
		break;
	case BLT:
		setOutput(&PORTK,&DDRK, 0);
		break;
	case BLB:
		setOutput(&PORTK,&DDRK, 1);
		break;
	}

}

/**
	* @brief Method for selecting the right output port with the stripPosition attribute in the Object
	* This is based on the mapping done for our drone.
	* This is the place to change the IO pins if we change the pins that control the LED strips
	* @param *port: Pointer to the port (e.g. PORTA) that will be used on Atmega2560
	* @param *ddr: Pointer to the DDR register (e.g. PORTA) that will be used on Atmega2560
	* @param pin: The number of the pin of the defined port on the Atmega 2560
	* @retval none
	* @note Example of utilisation setOutput(&PORTA, &DDRA, 3)
	*/
void WS2812::setOutput(volatile uint8_t *port, volatile uint8_t *ddr, uint8_t pin) 
{
	//seem to have problems with port F, except for F3..., can`t do shit
		pinMask = _BV(pin);
		ws2812_port = port;
		ws2812_port_reg = ddr;

}

/**
	* @brief resets the values in the pixel array of the object to 0 (must use sync to actually update the LED)
	* @param none
	* @retval none
	*/
void WS2812::reset()
{
	int ledNumber3x = count_led*3;
	for(int i = 0; i < ledNumber3x; i++)
	{
		pixels[i] = 0;
	}
}