/*****************************************************************************
 * 	      File: LabFinalProj.c
 *    Lab Name: Zumo Sumo AVR Controller
 *      Author: Prof. John Hutton
 *     Created: 12/30/17
 * Modified by: <Your Name
 *    Modified: 3/28/19, 1/17/21, 11/13/2023
 *   Processor: ATmega128A (on the ReadyAVR board)
 *
 * This is a placeholder for your final project program.  This will
 * leverage off Lab09, Lab10 as well as other work we have done in this
 * class.
 *
 * This program depends on the UARTLibrary used in previous labs, one for 
 * Character debug to the PUTTY terminal and one to the BT Master.
 *
 * NB: The ReadyAVR-64 board uses UART1 as the default for programming,
 * freeing uart1 for external RS232 traffic (e.g. like adding the Bluetooth
 * transceivers in Lab09).
 *   UART1 for communication to a Putty Terminal
 *   UART0 for communication across the paired BT channel.
 *
 * <Student Notes on program here>
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define END_STRING  '\0'
#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>  // for sprintf functionality
#include "UARTLibrary.h"
#include "UART0Library.h"

#define BAUDRATE   9600 // standard values from 1200 to 115200
#define DATABITS      8 // 5-8
#define STOPBITS      1 // 1,2
#define PARITY        0 // 0=none, 1=odd, 2=even

char text[40];

void send_string(const char *cp) {
	while(*cp != END_STRING) {
		uart_tx(*cp++);
	}
}

int main(void) {
	char rcvdChar;
	const char resetMsg[] = "\r\nRS232/BT Test Reflector.\r\n\0";
	
	// For simplicity, use the same setup information for both ports.
	// Init UART1 for debug information to Putty
    uart_init(BAUDRATE,DATABITS,STOPBITS,PARITY);
	//uart_tx('a');
	
	// Init UART0 for BT/UART path
	uart0_init(BAUDRATE,DATABITS,STOPBITS,PARITY);
	//while(1) {
		//uart0_tx('a');
		//_delay_ms(1000);
	//}
	
	send_string(resetMsg);
	
	while(1)
	{
		// wait for a character
		rcvdChar = uart_rx();
		
		switch(rcvdChar)
		{
			case CR:
				// send back both CR and LF (makes PuTTY work without having to assume CRs w/ LFs)
				uart_tx(CR);	// echo char back to sender
				uart_tx(LF);	// echo char back to sender
				break;					
			default:
				// Echo normal character back to terminal
				uart_tx(rcvdChar);
				// Send character to BT Device (Zumo)
				uart0_tx(rcvdChar);
		}
	}
	return 0;
}
