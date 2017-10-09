/*
 * shiftregister.h
 * 
 * Created: 10.02.2015 16:13:22
 *  Author: lweber
 */ 


#ifndef SHIFTREGISTER_H_
#define SHIFTREGISTER_H_
#include <stdio.h>
#include <avr/io.h>
#include "init.h"
#include "Konstanten.h"



uint16_t read_resolver();

void write_status(uint8_t status);

#endif /* SHIFTREGISTER_H_ */