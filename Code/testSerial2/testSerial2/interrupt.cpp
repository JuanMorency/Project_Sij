﻿#include "interrupt.h"

bool flagLCD = 0;
bool flagRF = 0;
bool flagESC = 0;
bool flagWS2812 = 0;
bool flagIMU = 0;

/*
PCINT0 is unused
PCINT1 is used for RF controller
*/
bool button_rising = false;
bool button_falling = false;

/**
	* @brief Sets the registers for timer 0 for interrupt control and starts interrupts
    * @param none
	* @retval None
	*/
void startInterrupt()
{
	/*by setting the max count of the counter (OCR0A) and the prescaler (with TCCR0B), the frequency of the interrupts
	generated by the counter can be controlled with precision */
	OCR0A = 0xFF;	/*Sets the maximum value of the counter before going back to 0*/
	TCCR0B |= (1 << CS00); /*prescaler 001 = 1, 010 = 8, 011 = 64, 100 = 256, 101 = 1024*/
	TCCR0A |= (1 << WGM01); /*Sets the mode of the counter to CTC to clear counter once OCR0A is reached*/
	//TIMSK0 |= (1 << OCIE0A); /*enable interrupt when counter reaches OCR0A*/
	sei();	/* Turn interrupts on */
}

//
//
///**
	//* @brief ISR for PCINT0 (button press)
    //* @param none
	//* @retval None
	//*/
//ISR(PCINT0_vect) {
	////read pin PB4 and if 1 this must be a rising edge change
	//if (PINB & 0b00010000) button_rising = true;
	//else button_falling = true;
	//_delay_ms(20); //debounce
//}
//
//
/**
	* @brief ISR for reading the pwm from the radio receiver. How it works: When the interrupt is detected
	* on a pin change, checks which channel triggered it by storing all the values of the pins and then checking which
	* one changed. To measure the width of the pulse, a counter (2 MHz) is used to measure the time it took
	* from a rising edge to a falling edge. This pulse width is the important information and is stored in the
	* ch_x_pw variables. The counter always runs to avoid having issues when 2 channels are triggerred at the same time
	* and overflow protection is implemented. 
    * @param none
	* @retval None
	*/
ISR(PCINT1_vect) {
	PORTC = 0x08;
	if(RFInitialized)
	{
		flagRF = 1;
	}
}

///**
	//* @brief ISR for reading the interrupt generated by the IMU
    //* @param none
	//* @retval None
	//*/
//ISR(INT2_vect) {
	//if(InertMUInitialized)
	//{
		//flagIMU = 1;
	//}
//}
//
///**
	//* @brief ISR for reading the interrupt generated by the USART 
    //* @param none
	//* @retval None
	//*/
//ISR(USART0_RX_vect) {
	//if(serialInitialized)
	//{
		//serialReceive();
	//}
//}
//
///**
	//* @brief ISR for reading the interrupt generated by the USART
    //* @param none
	//* @retval None
	//*/
//ISR(USART0_TX_vect) {
	//if(serialInitialized)
	//{
		//serialTransmitINT();
	//}
//}
//
/////**
	////* @brief ISR for the overflow on the counter (may not be necessary)
    ////* @param none
	////* @retval None
	////*/
////ISR(TIMER3_OVF_vect) {
	////timer_3_ovf=true;
	////TIFR3 &= ~(1 << TOV3);
////}
//
//
/**
	* @brief ISR for the event counter. This sets the different flags at different frequencies 
	in order to control different hardware. This is where to add different flags if you want more interrupt
	routines
    * @param none
	* @retval None
	*/
ISR(TIMER0_COMPA_vect){
	PORTC = 0x01;	
	static int LCDFlagCount;
	static int ESCFlagCount;
	static int WS2812FlagCount;
	//static int IMUFlagCount;
	//if(lcdInitialized == true){
		//if(LCDFlagCount >= LCD_PERIOD){
			//flagLCD = 1;
			//LCDFlagCount = 0;
		//}
		//else {
			//LCDFlagCount++;
		//}
	//}
//
	//if(escInitialized == true){	
		//if(ESCFlagCount >= ESC_PERIOD){
			//flagESC = 1;
			//ESCFlagCount = 0;
		//}
		//else {
			//ESCFlagCount++;
		//}
	//}
//
	//if(WS2812Initialized == true){
		//if(WS2812FlagCount >= WS2812_PERIOD){
			//flagWS2812 = 1;
			//WS2812FlagCount = 0;
		//}
		//else {
			//WS2812FlagCount++;
		//}
	//}

	//if(InertMUInitialized == true){
		//if(IMUFlagCount >= IMU_PERIOD){
			//flagIMU = 1;
			//IMUFlagCount = 0;
		//}
		//else {
			//IMUFlagCount++;
		//}
	//}
}