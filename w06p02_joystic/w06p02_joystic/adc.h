/*
 * adc.h
 *
 * Created: 2020-07-29 오전 11:10:13
 *  Author: user
 */ 


#ifndef ADC_H_
#define ADC_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#define ipin PINF
#define iport PORTF
#define ADC_VREF_TYPE 0x20

void ADC_init();
int ADC_read(char channel);

#endif /* ADC_H_ */