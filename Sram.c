/*
 * Sram.c
 *
 * Created: 16.01.2015 08:57:23
 *  Author: lweber
 *	S-RAM = 23LCV1024
 */ 
#include "Sram.h"


//****************************************** SAVE DATA IN S-RAM****************************************
void save_data(uint16_t adresse,uint16_t x,uint16_t y,uint16_t z)		//Wird im Byte Operation mode ausgelesen
{
	uint8_t speicher[10] = {2,0,0,0,0,0,0,0,0,0};	//Variable für das S-RAM Write und speicher 0
		
	uint8_t temp=0;
	PORTC &= ~(1<<CS_SRAM);					//CS S-RAM einschalten
	speicher[9]= z & 0xff;				//Low byte Z Hall
	speicher[8]= z>>8;					//high byte z Hall
	speicher[7]= y & 0xff;				//low byte y Hall
	speicher[6]= y>>8;					//high byte Y Hall 
	speicher[5]= x & 0xff;				//low byte x Hall
	speicher[4]= x>>8;					//High byte x Hall
	speicher[3]= adresse & 0xFF;		//low byte der Adresse
	speicher[2]= adresse>>8;			//highbyte der Adresse
	
	for (temp=0;temp<10;temp++)
	{		
		SPDR=speicher[temp];			//Senden von SPI Adresse und Daten
		while(!(SPSR & (1<<SPIF)));		//Warten bis übertragung fertig ist
	}
	PORTC |= (1<<CS_SRAM);			//CS S-RAM auschalten
}
//****************************************** SAVE DATA IN S-RAM****************************************

//********************************** READ RESOLVER DATA*************************************************
void read_res_time(uint16_t anzahl)
{
	uint8_t speicher[4] = {3,0,0,0};	//Variable für das S-RAM READ und speicher 0
	
	//send_word(anzahl);					//sende Wieviel Daten das gesendet wurden.
	_delay_ms(WAIT_TIME);				//Computer Zeit geben
	
	PORTC &= ~(1<<CS_SRAM);				//CS S-RAM einschalten
	_delay_us(0.25);
	uint16_t temp=0;
	
	for (temp=0;temp<4;temp++)			//Senden von Speicher Instruction
	{
		SPDR=speicher[temp];
		while(!(SPSR & (1<<SPIF)));		//Warten bis übertragung fertig ist
	}
	
	for (temp=0;temp<anzahl;temp++)			//per UART Daten senden Soviel wie in Parameter Anzahl angegeben wurden
	{
		uint8_t high;
		uint8_t low;
		int data;
		uint8_t i;
		for (i=0;i<3;i++)
		{
			switch (i)
			{
			case 0:uart_puts("X:");break;
			case 1:uart_puts("Y:");break;
			case 2:uart_puts("Z:");break;
			}

			SPDR=DUMMYDATA;					//irgendwas senden um übertragung zu starten
			while(!(SPSR & (1<<SPIF)));		//Warten bis übertragung fertig ist
			high=SPDR;						//High byte
		
			SPDR=DUMMYDATA;					//irgendwas senden um übertragung zu starten
			while(!(SPSR & (1<<SPIF)));		//Warten bis übertragung fertig ist
			low=SPDR;						//Senden des Wertes
			
			data=(high<<8) | low;
			send_sword(data);

		}
		/*if (temp<2)
		{
			_delay_ms(WAIT_TIME);		//Nach den ersten zeichen 100ms warten damit Labview genügend Zeit hat.
		}
		*/	
	}
	PORTC |= (1<<CS_SRAM);					//CS S-RAM auschalten
	
}
//********************************** READ RESOLVER DATA*************************************************

//********************************** SET MODE IN S-RAM*************************************************
void set_mode(uint8_t modus)
{
	PORTC &= ~(1<<CS_SRAM);			//CS S-RAM einschalten
	_delay_us(0.25);
	
	SPDR=1;						//Set Modus
	while(!(SPSR & (1<<SPIF)));	//Warten bis übertragung fertig ist
	SPDR=modus;					//Modus setzen
	while(!(SPSR & (1<<SPIF)));	//Warten bis übertragung fertig ist
	
	PORTC |= (1<<CS_SRAM);			//CS S-RAM auschalten
}
//********************************** SET MODE IN S-RAM*************************************************

//********************************** READ MODE IN S-RAM*************************************************
uint8_t read_mode()
{
	PORTC &= ~(1<<CS_SRAM);			//CS S-RAM einschalten
	_delay_us(0.25);
	
	SPDR=5;						//READ Modus
	while(!(SPSR & (1<<SPIF)));	//Warten bis übertragung fertig ist
	SPDR=0;
	while(!(SPSR & (1<<SPIF)));	//Warten bis übertragung fertig ist

	PORTC |= (1<<CS_SRAM);			//CS S-RAM auschalten
	return(SPDR);
}
//********************************** READ MODE IN S-RAM*************************************************