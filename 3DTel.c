/*
* Resolverausleser.c
* Programm um Position des Resolvers vom DDC Print in S-ram zu latchen bei
* einer Positionänderung des Encoders. Gemessen wird von Z zu Z Impuls
*
*Messfrequenz: 7uS -140kHz
*
* Created: 26.03.2015 16:00
*  Author: lweber
*/


#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#ifndef F_CPU
#define F_CPU 14745600
#endif

#include <util/delay.h>
#include "shiftregister.h"
#include "RS232.h"
#include "init.h"
#include "Sram.h"
#include "Encoder.h"
#include "Modus.h"
#include "FH5401C.h"

volatile uint8_t status;					//status des Programms
volatile uint16_t bytecount;				//Zählt wiviel bytes gezählt werden
volatile uint16_t counteralt;
volatile uint16_t counter;					//counter 16bit gross
volatile uint16_t speicher;					//Speicher Adresse SRAM
volatile uint8_t temp_counter;				//Temporärer Counter zählt die Messpunkte 

volatile uint8_t resolution;				//standart 700 Messpunkte

volatile uint8_t encodersig;

//*******************************************************Interrupts*********************************************
//Interrupt Flanke an INT0 A&B Signal Encoder
ISR (INT0_vect)
{
	encodersig=Encoderdata(encodersig);												//Verarbeitung der Encoder daten
	
	//PORTC ^= (1<<PC4);		//bit invertieren zur lauftzeit messung
		if ((encodersig & SOLLDREHRICHTUNG)==((encodersig&ISTDREHRICHTUNG)<<1))		//richtige Richtung?
		{
			counter++;
			if (counter>counteralt)
			{
				if(status==2 && temp_counter>= resolution)										//Status 2 = Messen und speichern
				{
					status=100;														//Status 100 = messen und speichern
					temp_counter=0;
					
					uint16_t timer=0;
					timer=TCNT1;
					TCNT1=0;				//Timer 1 auf 0
					
					if (timer<18)		//18 entspricht 1.25 mS. Der Sensor benötigt min. ca. 1mS
						{
							write_status(LED_ON);								// Alle LEDS an entspricht fehler
							counter=0;												//zähler reseten
							counteralt=0;
							temp_counter=16;
							speicher=0;
							status=0;
						}
				}
				temp_counter++;
				counteralt++;
			}	
		}
		else			//falsche Richtung
		{
			counter--;
		}

}

//Interrupt Funktion Positive Flanke an INT1 Z Signal Encoder
ISR (INT1_vect)
{
	if ((status==2 || status==100) && counter>(ENCODERPULSE-10))
	{
			cli();
			status=status+1;											//Messung erfolgreich -> Status 3 ODER 101 Auslesen der Daten
	}
	
	if ((status==0 || status==1) && counter>1520 && counter<1680)				//Zwischen 9.5 und 10.5 Index Signal
	{
		uint8_t temp=0;
		write_status(LED_OFF);										//Alle LED aus
		temp=(encodersig&ISTDREHRICHTUNG)<<1;
		encodersig=(encodersig&0b01111111)|temp;				//Aktuelle Drehrichtung = Soll Drehrichtung
		counter=0;												//zähler reseten
		counteralt=0;
		temp_counter=16;
		speicher=0;													//Alles auf 0 setzen
		status=2;												//Status 2 Aufnahme der Ergebnisse
		//write_status(YELLOWLED_1);
		TCNT1=20;				//Timer 1 auf 20
	}
}
//**********************************************Funktionen*******************************************

//SENDEN VON WERTEN FUNKTION
void auslesen(void)
{
	write_status(LED_ON);
	char string[] = "Messwerte:\n";
	uart_puts(string);
	init_sram();
	read_res_time(speicher);					//Werte auslesen
	status=0;
	char string2[] = "END\n";
	uart_puts(string2);
	sei();
}

void zero()
{
	//write_status(GREENLED_2);
	counter=0;												//zähler reseten
	counteralt=0;
	temp_counter=16;
	speicher=0;
}


//Einzelne Mesung durchführen

void measure()
{
	int z,x,y;
	singleshot(&x,&y,&z);
	save_data(speicher*6,x,y,z);
	speicher++;														// Speicheradresse erhöhen
	status=2;
}

void check_drehrichtung()
{
	if ((status==0||status==1) && (counter>ENCODERPULSE))						//ändern der drehrichtung
	{
		uint8_t temp=0;
		temp=(encodersig&ISTDREHRICHTUNG)<<1;
		encodersig=(encodersig&0b01111111)|temp;				//Aktuelle Drehrichtung = Soll Drehrichtung
		zero();
	}
}

void check_button()
{
	if (!(PINC & (1<<PINC6)))									//Button gedrückt
	{	
		char string[] = "SAVE\n";								//Schreibe Save damit gespeichert wird
		uart_puts(string);
	}
}

//EMPFANG FUNKTION
void uartempfangen (void)
{
	uint8_t empfang;
	
	empfang=check_uart();
	switch(empfang)
	{
		case 'g': status=0;singlemeasurement();break;		//einzelne Messung
		case 'r': status=0;reset_hallsensor();break;		//Reset Hallsonde
		case 'i': UDR=status;break;							//Status abfragen
		case 'o': send_sword(speicher);break;
		case 'c': send_word(counter);break;					//send_word(counter)
		case 's': status=0;speed_test();break;
		case 'k': status=0;temperatur();break;
		case 't': status=0;Testmodus();break;				//Testmodus der LED
		case 'm': status=1;zero();break;					//Warte auf Z Impuls
		case 'u': status=0;break;							//Messung abgebrochen
		case 'h':status=0;IDN();break;						//Anfrage COMPORT Labview
		case '1':write_status(G_T_LED);break;					
		case '2':write_status(B_T_LED);break;
		case '3':write_status(GOOD_LED);break;
		case '4':write_status(BAD_LED);break;
		case '5':write_status(255);break;
		case '6':write_status(0);break;
		case 'Q':status=0;resolution= sehrhoch;break;		//Einstellung der Messpunkte
		case 'W':status=0;resolution= hoch;break;
		case 'E':status=0;resolution= mittelhoch;break;
		case 'R':status=0;resolution=mittel;break;
		case 'T':status=0;resolution=niedrig;break;
		case   0: break;									//nichts empfangen nichts machen
		default: UDR='R';status=0;break;					//Fehler-> Falsches Zeichen empfangen
	}
}

//ENDE FUNKTIONEN
int main(void)
{
	while(1)
	{
		init();
		status = 0;
		counter=0;
		write_status(LED_ON);								// Alle LEDS an
		resolution=16;
		sei();
		while (1)										// unentliche schleife
		{
			
			check_drehrichtung();								//überprüfe soll drehrichtung
			
			switch(status)
			{
				case 0: uartempfangen();break; //check_button();break;	// warten bis LABVIEW bereit
				case 1: uartempfangen();break;					//Counter auf 0 Setzen für neue Messung
				case 2: uartempfangen();break;					//Messung im gange
				case 3: auslesen();break;						//Daten zurückschicken auslesen
				case 100:measure();break;
				case 101:auslesen();break;
				//case 90: datenaufnahme();break;
				default: status=0;break;						//default = neustart
			}
			
		}
	}
	return 0;
}

