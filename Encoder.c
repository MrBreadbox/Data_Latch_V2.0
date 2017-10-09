/*
 * Encoder.c
 *
 * Created: 06.04.2015 10:16:29
 *  Author: Lukas
 */ 
#include "Encoder.h"


uint8_t Drehrichtung(uint8_t encodersig)			//Überprüft die Drehrichtung
{	
	uint8_t rechtslauf=FALSE;						//Variable True=1= Rechtslauf
	uint8_t altakt=encodersig & AB_ALT_NEU;			//Nur altes und neues AB Signal
	
	switch (altakt)
	{
		case 1:	rechtslauf=TRUE;	break;
		case 2:	rechtslauf=FALSE;	break;
		case 4:	rechtslauf=FALSE;	break;
		case 7:	rechtslauf=TRUE;	break;
		case 8:	rechtslauf=TRUE;	break;
		case 11:rechtslauf=FALSE;	break;
		case 13:rechtslauf=FALSE;	break;
		case 14:rechtslauf=TRUE;	break;
		default:rechtslauf=TRUE;	break;
	}
	
	return(rechtslauf);								//Rechtslauf= 64
}


uint8_t Encoderdata(uint8_t encodersig)
{
	uint8_t temp;
	uint8_t temp2;
	
	temp=(PINC & PORTC_AB)>>SHIFT_AB;									//AB Signal einlesen und auf bit 0&1 schieben
	temp2=encodersig & ~(AB_NEU);										//Neus AB Signal in encoder sig einfügen 0b11111100
	encodersig=temp|temp2;												//Aktuelles AB Signal in encoder sig einfügen
	temp=Drehrichtung(encodersig);
	temp2=encodersig & ~(ISTDREHRICHTUNG);								//(0b10111111)
	encodersig= temp|temp2;												//Nur das Ist drehrichtungs Bit setzen
	encodersig= (encodersig&~(AB_ALT))|((AB_NEU & encodersig)<<2);		//Aktuelles AB Signal auf Bit 2&3 verschieben

	return(encodersig);													//verarbeitete Daten wider ausgeben
}

/*
Byte Encodersig Definition:

Bit7:	Solldrehrichtung	(wird beim Z Punkt bestummen)
Bit6:	Istdrehrichtung
Bit5:	Z_Event				(Falls das Z Signal auftritt bevor AB =1,1 ist auf 1 setzen)
Bit4:	-
Bit3:	Altes A Signal Encoder (Alt = Vor neuem Inkrement)
Bit2:	Altes B Signal Encoder
Bit1:	Neues A Signal Encoder	(Neu= Aktuelles Inkrement)
Bit0:	Neues B Signal Encoder
*/