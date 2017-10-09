/*
 * SPI.h
 *
 * Created: 24.03.2016 16:44:44
 *  Author: lweber
 */ 

#ifndef SPI_H_
#define SPI_H_
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

uint8_t send_read_spi(uint8_t data);

uint8_t read_spi();

#endif /* SPI_H_ */