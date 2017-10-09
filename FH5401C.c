/*
 * FH5401C.c
 * 3D Hall Sensor ansteuerung
 * Created: 24.03.2016 13:48:58
 *  Author: Lukas Weber
 */

#include "FH5401C.h"

void singleshot(int *z, int *x, int*y )
{
	PORTB &= ~(1<<CS_FH5401);			//CS Hallsensor einschalten
	//PORTC |= (1<<PC3);					//Laufzeitmessung
	
	send_read_spi(0x80);
	send_read_spi(0x0E);
	send_read_spi(0x00);
	send_read_spi(0x01);				//Modus Singleshot

	while ( !(PINC & (1<<READY_FH5401))); //CHECK READY SIGNAL
	
	send_read_spi(0x01);
	send_read_spi(0x22);
	send_read_spi(0x01);
	send_read_spi(0x10);				//Ready Signal reset

	result(HALL_Z);
	*z = result(HALL_X);
	*x = result(HALL_Y);
	*y = result(HALL_Z);
	
	
	//PORTC &=  ~(1<<PC3);				//Laufzeitmessung
	PORTB |= (1<<CS_FH5401);			//CS Hallsensor auschalten
}

void singlemeasurement()
{
	int z,x,y;
	singleshot(&x,&y,&z);
	
	UDR = 'Z';							
	while(!(UCSRA & (1<<UDRE)));		// Warten bis senden bereit
	send_sword(z);
	UDR = 'X';
	while(!(UCSRA & (1<<UDRE)));		// Warten bis senden bereit
	send_sword(x);
	UDR = 'Y';
	while(!(UCSRA & (1<<UDRE)));		// Warten bis senden bereit
	send_sword(y);
	UDR = 10;							// Ende String Zeichen
	while(!(UCSRA & (1<<UDRE)));		// Warten bis senden bereit
	
}

int result(uint8_t speicher)
{
	int data;
	int data2;
	
	data=send_read_spi(0x01);
	data=data<<8;
	data2=send_read_spi(speicher);
	data=data | data2;
	return(data);	
}

void temperatur()
{
	PORTB &= ~(1<<CS_FH5401);			//CS Hallsensor einschalten
	uint8_t highb;
	uint8_t lowb;
	int antwort;
	int z,x,y;
	singleshot(&x,&y,&z);
	PORTB &= ~(1<<CS_FH5401);			//CS Hallsensor einschalten

	send_read_spi(0x01);
	send_read_spi(0x10);

	highb=send_read_spi(0x01);
	lowb=send_read_spi(0x10);
	
	antwort=lowb | (highb<<8);
	char string[] = "Temp.:\n";
	uart_puts(string);
	send_sword(antwort);
	PORTB |= (1<<CS_FH5401);			//CS Hallsensor auschalten
}

void reset_hallsensor()
{
	PORTB &= ~(1<<CS_FH5401);			//CS Hallsensor einschalten
	PORTB &= ~(1<<RESET_FH5401);		//RESET Ein
	_delay_ms(500);
	PORTB |= (1<<RESET_FH5401);			//RESET Aus
	_delay_ms(10);
	send_word(check_ready());
	init_5401();
	PORTB |= (1<<CS_FH5401);			//CS Hallsensor auschalten
}

uint16_t check_ready()
{
	uint8_t highb;
	uint8_t lowb;
	uint16_t antwort;
	PORTB &= ~(1<<CS_FH5401);			//CS Hallsensor einschalten
	send_read_spi(0x01);
	send_read_spi(0x07);
	highb=send_read_spi(0x01);
	lowb=send_read_spi(0x07);
	antwort=lowb|(highb<<8);
	PORTB |= (1<<CS_FH5401);			//CS Hallsensor auschalten
	return(antwort);
}
