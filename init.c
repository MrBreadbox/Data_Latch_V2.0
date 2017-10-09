/*
 * init.c
 * Initialisierung
 * Created: 05.04.2015 13:48:58
 *  Author: Lukas Weber
 */

// Hilfsmakro zur UBRR-Berechnung ("Formel" laut Datenblatt)
#define UART_UBRR_CALC(BAUD_,FREQ_) ((FREQ_)/((BAUD_)*16L)-1)
#include "init.h"

void init()
{
	//Serielle Schnittstelle einrichten
	UBRRH = (uint8_t)( UART_UBRR_CALC( UART_BAUD_RATE, F_CPU ) >> 8 );
	UBRRL = (uint8_t)UART_UBRR_CALC( UART_BAUD_RATE, F_CPU );
	// Frameformat = 8 Bit
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);

	//Datenrichtungsregister
	DDRA = 0b10000000;							//PA7 = SHIFT_OUT für Status LED Shift register 
	DDRB = 0b10111110;							//PB0=SSTRB,PB1=CSAD,PB2=Reset 3D,PB3=CE_IN(not used),PD4=CS_3D,REST = SPI
	DDRC = 0b00001100;							//PC2=CLK 3D-Sensor PC3=CS SRAM,PC4= Ready 3D-Sens,
	DDRD = 0b01000010;							//PD0=RXD, PD1=TXD, PD2=AORB PD3=Z,PD4=A, PD5=B,PD6=PL ,PD7=B2
		
	PORTB |= (1<<CS_FH5401);					//CS 3D Hallsensor abschalten
	PORTC |= (1<<CS_SRAM);						//CS S-RAM abschalten
	PORTB |= (1<<CS_AD);						//CS AD Wandler
	PORTB |= (1<<PB3);							//CS Schieberegister Input (keine Funktion in V 2.0
	PORTC |= (1<<PC5);							//Button Pull Up Widerstand
	UCSRB = (1 << TXEN)|(1 << RXEN);			// Setzt TXEN und RXEN Bit= Transmitter Enable Reciver enable
	SPSR = (1<<SPI2X);							// Douple speed aktivieren
	SPCR = SPI_CONFIG_SRAM;						//SPI ENABLE,Master auf 1 rest =0

	PORTB &= ~(1<<RESET_FH5401);				//Reset deaktivieren 3D Sensor
	PORTB &= ~(1<<PB3);							//ENABLE vom Resolver Print
	
	OCR2 = 250;									//Setze das Compare register
	//TIMSK |= (1<<OCIE2);						//Compare Interrupt Timer 2 erlauben
	
	//Interrupt Eingänge Encoder
	MCUCR = (1<<ISC00)|(1<<ISC11)|(1<<ISC10);	// ISC00 auf 1 = jede änderung an INT 0 lösst einen Interrupt aus positivie Flanke INT 1 lösst Interrupts aus
	GICR = (1<<INT1)|(1<<INT0);	       // aktivieren von INT0 = AB Impuls INT 1 = Z Impuls
	
	set_mode(SEQUENTIAL_MODE);					//Sram MODE
	reset_hallsensor();
	init_5401();								//3D Sensor initialisieren
	TCCR1B=5;									//Timer 1 starten -> Prescaler 1024
	
}

void init_sram()
{
	//SPI EINRICHTEN FÜR SRAM
	SPCR = SPI_CONFIG_SRAM;		//SPI ENABLE,Master auf 1 rest =0
}

void init_165D()
{	
	//Für Eingang 165D LESE resolver Position
	//SPCR &= ~ ((1<<CPHA)|(1<<SPR0));
	//SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL);			//SPI ENABLE,MASTER, set clock rate fck/16,steigende Taktflanke = einlesen
	
	SPCR = SPI_CONFIG_165;		//SPI ENABLE,Master auf 1 rest =0
	
	/*SPDR = 213;										//Dummy Daten senden EVT. ÜNNÖTIG!!
	while (!(SPSR & (1<<SPIF)));					// warten bis übertragung fertig
	SPDR = 123;										//DUMMY DATEN 2
	while (!(SPSR & (1<<SPIF)));					// warten bis übertragung fertig
	*/
}

void init_595D()
{
	SPCR = SPI_CONFIG_595;
}

void IDN()			//Sende Antwort zurück damit Labview die richtige COM Schnitstelle erkennt.
{
	uint8_t i='l';						//Sende Werte lmnop von l-p alphabet als erkennung	lmnop	
	for (i='l';i<'q';i++)
	{
		UDR = i;
		while(!(UCSRA & (1<<UDRE)));	// Warten bis senden bereit
	}
	
}

void init_5401()
{
	PORTB &= ~(1<<CS_FH5401);			//CS Hallsensor einschalten	
	send_read_spi(0x80);
	send_read_spi(0x0B);
	send_read_spi(0x00);
	send_read_spi(0x00);				//Single Magnetic Probe
	
	PORTB |= (1<<CS_FH5401);			//CS Hallsensor auschalten
}