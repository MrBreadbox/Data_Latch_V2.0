/*
 * SPI.c
 *
 * Created: 24.03.2016 16:44:44
 *  Author: lweber
 */ 
#include "SPI.h"


uint8_t send_read_spi(uint8_t data)
{
	uint8_t antwort;
	SPDR = data;									//Daten senden
	while (!(SPSR & (1<<SPIF)));					// warten bis übertragung fertig
	antwort = SPDR;
	return(antwort);
}

uint8_t read_spi()
{
	//uint8_t antwort;
	while (!(SPSR & (1<<SPIF)));					// warten bis übertragung fertig
	//antwort = SPDR;
	return(SPDR);
}