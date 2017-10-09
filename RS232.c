/*
 * RS232.c
 *
 * Created: 10.02.2015 17:19:09
 *  Author: lweber
 */ 
#include "RS232.h"

void send_word(uint16_t wert)
{
	
	uint8_t low=0;
	uint8_t high=0;
	
	low=wert & 0xff;					//aufteilung in low und Highbyte
	high=wert >> 8;	
	
	UDR = low;							//Senden von Lowbyte
	while(!(UCSRA & (1<<UDRE)));		// Warten bis senden bereit
	
	UDR = high;
	while(!(UCSRA & (1<<UDRE)));		// Warten bis senden bereit
		
}

void send_sword(int wert)
{
	char Buffer[10];
	
	itoa(wert,Buffer,10);
	uart_puts(Buffer);
	while(!(UCSRA & (1<<UDRE)));		// Warten bis senden bereit
//	UDR = 10;							// Ende String Zeichen
//	while(!(UCSRA & (1<<UDRE)));		// Warten bis senden bereit
}

void uart_putc(unsigned char c)
{
	while(!(UCSRA & (1<<UDRE)));		// Warten bis senden bereit
	UDR = c;
}

void uart_puts (char *s)
{
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
}

uint8_t check_uart()
{
	 if ( (UCSRA & (1<<RXC)) )
	 {
		 return(UDR);		//Empfangenes Zeichen zurücksenden
	 }
	else
	{
		return(0);			//Falls nichts im Buffer enthalten ist.
	}
}