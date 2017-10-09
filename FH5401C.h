/*
 * FH5401C.h
 *
 * Created: 24.03.2016 10:16:09
 *  Author: Lukas
 */ 

#ifndef FH5401C_H_
#define FH5401C_H_


#ifndef F_CPU
#define F_CPU 14745600
#endif

#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include "Konstanten.h"
#include "SPI.h"
#include "RS232.h"
#include "init.h"


void singleshot();

void reset_hallsensor();

uint16_t check_ready();

void temperatur();

int result(uint8_t speicher);

void singlemeasurement();


 

#endif  /* FH5401C_H_ */