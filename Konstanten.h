/*
 * Konstanten.h
 *
 * Created: 08.04.2015 11:10:36
 *  Author: lweber
 */ 


#ifndef KONSTANTEN_H_
#define KONSTANTEN_H_

//*********************Kontanten Allgemein**********************************
#define ENCODERPULSE 11200		//Anzahl Pulse bis Encoder 360° erreicht. 70 Pole a 160 Pulse

#define SOLLDREHRICHTUNG 0b10000000
#define ISTDREHRICHTUNG 0b01000000
#define ENCODER_ZEICHEN 42662		//Speichert ein e,e im Speicher.
//*********************Kontanten Allgemein**********************************

//*********************STATUS_LED*******************************************
#define BAD_LED		192				//Bad LED
#define GOOD_LED	160				//Good LED
#define G_T_LED		32				//Good and Taster LED
#define B_T_LED		64				//Bad and Taster LED
#define B_G_LED		128
#define LED_ON		224
#define LED_OFF		0

#define REDLED_2	128
#define REDLED_1	64
#define YELLOWLED_4 32
#define YELLOWLED_3 16
#define YELLOWLED_2 8
#define YELLOWLED_1 4
#define GREENLED_2	2
#define GREENLED_1	1
//*********************STATUS_LED*******************************************

//*********************Modus.C**********************************
#define DELAY_LAUFLICHT_MS 100
#define RECHTS 1
#define LINKS 0
#define LETZTES_LED 128
#define ERSTES__LED 1
//*********************Modus.C**********************************

//*********************S-RAM************************************
#define SEQUENTIAL_MODE 64
#define PAGE_MODE 128
#define BYTE_MODE 0
#define SPI_CONFIG_SRAM 0b01010000;		//SPI ENABLE,Master auf 1 rest =0
#define WAIT_TIME 100					//Wartezeit in ms um Labview etwas zeit zu geben
#define CS_SRAM PC3
//*********************S-RAM************************************

//*********************Shiftregister************************************
#define DUMMYDATA 213		//irgendeine Zahl
#define SPI_CONFIG_595 0b01010000;		//SPI ENABLE,Master auf 1 rest =0
#define SPI_CONFIG_165 0b01010000;		//SPI ENABLE,Master auf 1 rest =0
//*********************Shiftregister************************************

//******************ENCODER*******************************************
#define TRUE 64
#define FALSE 0
#define AB_NEU 0b00000011
#define AB_ALT 0b00001100
#define AB_ALT_NEU 0b00001111
#define PORTD_AB 0b00110000
#define PORTC_AB 0b01100000				//Optionaler Encoder durch port C AB ersetzen. (V2.0 = 0b00000011)
#define SHIFT_AB 5						//5 mal rechts schieben um wieder auf Bit 0&1 zu gelangen.
#define Z_BIT	0b00100000
//******************ENCODER*******************************************

//******************FH5401C*******************************************
#define CS_FH5401 PB4
#define RESET_FH5401 PB2  
#define READY_FH5401 PC4
#define CLK_FH5401 PC2
#define TEMP 0x10
#define HALL_Z 0x12
#define HALL_X 0x13
#define HALL_Y 0x14

//Auflösung 
#define sehrhoch 10		//1120 Messpunkte
#define hoch 16				// 700 Messpunkte
#define mittelhoch 20		// 560 Messpunkte
#define mittel 25			// 448 Messpunkte
#define niedrig 28			//400Messpunkte 


//******************FH5401C*******************************************

//******************MAX1300*******************************************
#define CS_AD PB1		
//******************MAX1300*******************************************
#endif /* KONSTANTEN_H_ */