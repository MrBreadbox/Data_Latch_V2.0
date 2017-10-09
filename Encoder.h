/*
 * Encoder.h
 *
 * Created: 06.04.2015 10:16:09
 *  Author: Lukas
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include "Konstanten.h"
#include "shiftregister.h"

uint8_t Drehrichtung(uint8_t encodersig);

uint8_t Encoderdata(uint8_t encodersig);

#endif /* ENCODER_H_ */