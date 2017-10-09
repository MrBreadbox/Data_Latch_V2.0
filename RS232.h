/*
 * RS232.h
 *
 * Created: 10.02.2015 17:18:55
 *  Author: lweber
 */ 


#ifndef RS232_H_
#define RS232_H_
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void send_word(uint16_t wert);

void send_sword(int wert);

void uart_putc(unsigned char c);

void uart_puts (char *s);

uint8_t check_uart();

#endif /* RS232_H_ */