#define F_CPU 16000000 // 8 MHz

/*Very Important - change F_CPU to match target clock 
  Note: default AVR CLKSEL is 1MHz internal RC
  This program transmits continously on USART. Interrupt is used for 
	Receive charactor, which is then transmitted instead. LEDs are used 
	as a test. Normal RX routine is included but not used.
  Change USART_BAUDRATE constant to change Baud Rate
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "interrupt.h"
#include "RF.h"

// Define baud rate
#define USART_BAUDRATE 57600   
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

volatile unsigned char value;  
/* This variable is volatile so both main and RX interrupt can use it.
It could also be a uint8_t type */

/* Interrupt Service Routine for Receive Complete 
NOTE: vector name changes with different AVRs see AVRStudio -
Help - AVR-Libc reference - Library Reference - <avr/interrupt.h>: Interrupts
for vector names other than USART_RXC_vect for ATmega32 */

ISR(USART_RXC_vect){
 
   value = UDR2;             //read UART register into value
   PORTB = ~value;          // output inverted value on LEDs (0=on)
}

void USART_Init(void){
   // Set baud rate
   UBRR2L = BAUD_PRESCALE;// Load lower 8-bits into the low byte of the UBRR register
   UBRR2H = (BAUD_PRESCALE >> 8); 
	 /* Load upper 8-bits into the high byte of the UBRR register
    Default frame format is 8 data bits, no parity, 1 stop bit
  to change use UCSRC, see AVR datasheet*/ 

  // Enable receiver and transmitter and receive complete interrupt 
  UCSR2B = ((1<<TXEN2)|(1<<RXEN2) | (1<<RXCIE2));
}


void USART_SendByte(uint8_t u8Data){

  // Wait until last byte has been transmitted
  while((UCSR2A &(1<<UDRE2)) == 0);

  // Transmit data
  UDR2 = u8Data;
}


// not being used but here for completeness
      // Wait until a byte has been received and return received data 
uint8_t USART_ReceiveByte(){
  while((UCSR2A &(1<<RXC2)) == 0);
  return UDR2;
}

void Led_init(void){
   //outputs, all off
	DDRC =0xFF;       
   PORTC = 0x02;        
}

int main(void){
   Led_init();    // init LEDs for testing
   USART_Init();  // Initialise USART
   initRF();
   //sei();         // enable all interrupts
   startInterrupt();

   value = 'A'; //0x41;    
   //PORTC = ~value; // 0 = LED on
   
   for(;;){    // Repeat indefinitely
             
     USART_SendByte(value);  // send value 
     _delay_ms(250);         
		         // delay just to stop Hyperterminal screen cluttering up    
   }
}