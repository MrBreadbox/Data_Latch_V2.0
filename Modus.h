/*
 * Modus.h
 *
 * Created: 07.04.2015 17:06:12
 *  Author: lweber
 */ 


#ifndef MODUS_H_
#define MODUS_H_

#ifndef F_CPU
#define F_CPU 14745600
#endif

#include "shiftregister.h"
#include "RS232.h"
#include "Sram.h"
#include <util/delay.h>
#include "Konstanten.h"


void Testmodus();			//LED Testmodus

void speed_test();			//Test wie schnell Daten aufgenommen werden können.


#endif /* MODUS_H_ */