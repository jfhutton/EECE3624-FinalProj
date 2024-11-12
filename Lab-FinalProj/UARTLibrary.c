/*****************************************************************************
 *	    File: UARTLibrary.c
 *  Lab Name: (Needed for Lab07 and Lab08)
 *    Author: Dr. Greg Nordstrom
 *   Created: 3/3/2019
 *  Modified: 
 *  Mod Date:
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This is intended to be compiled as an AVR Library Project and
 * used by other programs. After compiling this code, copy the
 * libUART1Library.a and UART1Library.h files to the project directory,
 * then add the library to the target project by R-clicking on the
 * project's Libraries directory and clicking Add Library. Also,
 * the UART1Library.h file must be #included in the project's .c file.
 *
 * NOTE: This library only works with the default UART1 port
 * on the ReadyAVR (so JP2 must have jumpers on PD3 and PD2).
 */

#include <avr/io.h>
#include "UARTLibrary.h"

// Use UART1_init to initialize the UART hardware in the AVR processor.
// The function takes the following arguments:
//    - desired baud rate (standard values from 2400 to 115200),
//    - number of databits (5-8),
//    - number of stop bits (1,2),
//    - parity (0=no parity, 1=odd parity, 2=even parity).
// No error checking is performed and the function does not return a value.
//
// For example,
//     uart1_init(9600,8,1,0);
// sets the baudrate to 9600, with 8 data bits, 1 stop bit, and no parity
void uart_init(long unsigned int baudrate, unsigned char databits, unsigned char stopbits, unsigned char parity) {
	// set baud rate (via UBRR0 high and low registers)
	int UBRR = F_OSC/(16*baudrate) - 1;
	UBRR1H = (UBRR >> 8);
	UBRR1L = UBRR;
    
	// set databits
	// for 5-8 bits, the 3-bit combination telling char size is bitsize - 5 (e.g. 8 bits is 011)
	unsigned char adjusted_databits = databits - 5;
	// top bit is always 0--stored in UCSR0B bit 2
	UCSR1B &= ~(1<<UCSZ02);
	// bottom two bits go in UCSR0C bits 1 and 2
	UCSR1C = (adjusted_databits << 1);
    
	// set parity
	if (parity > 0) { // even or odd--UPM01 high
		UCSR1C |= (1<<UPM01);
	} else { // disabled--UPM01 low
		UCSR1C &= ~(1<<UPM01);
	}
	if (parity == 1) { // odd--UPM00 high
		UCSR1C |= (1<<UPM00);
	} else { // disabled or even--UPM00 low
		UCSR1C &= ~(1<<UPM00);
	}
    
    // set stopbits
	if (stopbits == 1) {
		UCSR1C &= ~(1<<USBS0);
	} else {
		UCSR1C |= (1<<USBS0);
	}
    
	// finally, enable receiver and transmitter
	UCSR1B |= 1<<RXEN0 | 1<<TXEN0;
}

// This function waits for space in the UART transmit buffer. When space
// is available, the function places an unsigned character value (passed in
// as the function's only argument) in the buffer and returns. No error
// checking is performed and the function does not return a value.
void uart_tx(unsigned char data) {
	// wait until UDRE0 bit is set, meaning transmit buffer is ready
	while (!(UCSR1A & (1<<UDRE0)));
	UDR1 = data;
}

// This function blocks until a character is received in the UART input
// buffer. Once a character arrives, it is passed back to the caller as
// an unsigned character. No error checking is performed.
unsigned char uart_rx(void) {
	while (!(UCSR1A & (1<<RXC0)));
	return UDR1;
}