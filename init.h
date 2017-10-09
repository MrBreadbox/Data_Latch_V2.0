/*
 * init.h
 *	Initiallisierung
 * Created: 10.02.2015 13:48:17
 *  Author: lweber
 */ 


#ifndef INIT_H_
#define INIT_H_
//#include <avr/io.h>


#define UART_BAUD_RATE 230400
#ifndef F_CPU
#define F_CPU 14745600
#endif


#include <avr/io.h>
#include <stdio.h>
#include "Sram.h"
#include "Konstanten.h"
#include "FH5401C.h"


void init();

void init_sram();

void init_165D();

void init_595D();

void init_5401();

void IDN();

#endif /* INIT_H_ */