/*
* Modus.c
*
* Created: 07.04.2015 17:06:27
*  Author: lweber
*/

#include "Modus.h"

void Testmodus()
{	
	uint8_t temp=0;
	uint8_t ledtest=1;
	uint8_t laufrichtung=LINKS;
	temp=check_uart();
	while(temp==0)
	{
		write_status(ledtest);
		_delay_ms(DELAY_LAUFLICHT_MS);
		
		if (laufrichtung==LINKS)
		{
			ledtest=ledtest<<1;	//Nach links schieben
			if (ledtest==LETZTES_LED)
			{
				laufrichtung=RECHTS;
			}
		}
		else
		{
			ledtest=ledtest>>1;	//Nach Rechts schieben
			if (ledtest==ERSTES__LED)
			{
				laufrichtung=LINKS;
			}
		}
		
		temp=check_uart();		//Befindet sich im LED Testmodus bis ein Zeichen empfangen wird.
	}
	write_status(0);				//LED abschalten
}

void speed_test()
{
	uint16_t i=0;
	PORTD &= ~(1<<PD7);				//CS Schieberegister Resolver In einschalten
	
	PORTC |= (1<<PC7);						//PC0 auf 1 setzen TEST LAUFZEIT Oszilloskop Messung
	
	for (i=0;i<700;i++)
	{
		int z,x,y;
		singleshot(&x,&y,&z);
		save_data(i*6,i,i,i);
	}

	PORTC &= ~ (1<<PC7);					//PC0 auf 0 setzen Test Laufzeit
	PORTD |= (1<<PD7);						//CS Schieberegister Resolver In ausschalten
	
	init_sram();
	read_res_time(i*3);						//Werte auslesen *3 weil der Wert 3*16Bit gross ist
}