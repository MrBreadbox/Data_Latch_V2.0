/*
 * shiftregister.c
 *
 * Created: 10.02.2015 16:44:44
 *  Author: lweber
 */ 
#include "shiftregister.h"

void write_status(uint8_t status)
{
	//init_595D();
	
	SPDR=status;							//Sende highbyte Resolver position zu schiebgeregister
	while (!(SPSR & (1<<SPIF)));				// warten bis übertragung fertig
	
	//0-1-0 Puls an STCP von Schieberegister um Werte zu übernehmen
	PORTA &= ~ (1<<PA7);
	PORTA |= (1<<PA7);
	PORTA &= ~ (1<<PA7);
	
}


uint16_t read_resolver()
{
	uint8_t wertl;
	uint8_t werth;
	//init_165D();
	PORTD &= ~(1<<PD7);								//CS Schieberegister Resolver In einschalten
	
	PORTD &= ~(1<<PD6);								//pl von Schieberegister einmal aus und ein
	PORTD |= (1<<PD6);								//parallel laden
	SPDR = DUMMYDATA;								//Dummy Daten senden
	while (!(SPSR & (1<<SPIF)));					// warten bis übertragung fertig
	werth = SPDR;									// erste 8 Bit von Resolver Position HIGHBYTE

	SPDR=DUMMYDATA;									//Dummy Daten senden

	while (!(SPSR & (1<<SPIF)));					// warten bis übertragung fertig
	wertl = SPDR;									// zweite 8 Bit von Resolver Position

	PORTD |= (1<<PD7);								//CS Schieberegister Resolver In ausschalten
	
	return(wertl + (werth <<8));				//zurücksenden des Wertes
}