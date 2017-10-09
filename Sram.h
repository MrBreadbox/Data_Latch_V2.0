/*
 * Sram.h
 *
 * Created: 05.04.2015 08:51:57
 *  Author: lweber
 */ 

#ifndef SRAM_H_
#define SRAM_H_

#ifndef F_CPU
#define F_CPU 14745600
#endif

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "Init.h"
#include "RS232.h"

void read_data(uint16_t anzahl);

void read_res_time(uint16_t anzahl);

void save_data(uint16_t adresse,uint16_t x,uint16_t y,uint16_t z);

uint8_t read_mode();

void set_mode(uint8_t modus);



#endif /* SRAM_H_ */