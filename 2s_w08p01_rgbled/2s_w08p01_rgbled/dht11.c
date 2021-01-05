/*
 * dht11.c
 *
 * Created: 2020-12-09 오전 10:30:34
 *  Author: Yunseo Hwang
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "dht11.h"

#define BV(n) (1 << (n))

struct dht11_port dht_port; 

void set_dht11_port(struct dht11_port port)
{
	dht_port.ddr = port.ddr;
	dht_port.port = port.port;
	dht_port.pin = port.pin;
	dht_port.input = port.input;
}

uint8_t dht11_getdata(uint8_t select)
{
	static uint8_t bits[5] = {0, };
	uint8_t i, j = 0;
	
	*(dht_port.ddr) |= BV(dht_port.input);
	*(dht_port.port) |= BV(dht_port.input);
	_delay_ms(100);
	
	*(dht_port.port) &= ~BV(dht_port.input);
	_delay_ms(18);
	*(dht_port.port) |= BV(dht_port.input);
	_delay_ms(1);
	*(dht_port.ddr) &= ~BV(dht_port.input);
	_delay_us(39);
	
	if ((*(dht_port.pin)) & BV(dht_port.input)) {
		return DHT11_ERROR;
	}
	_delay_us(80);
	if (!(*(dht_port.pin) & BV(dht_port.input))) {
		return DHT11_ERROR;
	}
	_delay_us(80);
	
	for (j = 0; j < 5; j++) {
		uint8_t result = 0;
		for (i = 0; i < 8; i++) {
			while(!(*(dht_port.pin) & BV(dht_port.input)));
			_delay_us(30);
			if(*(dht_port.pin) & BV(dht_port.input)) {
				result |= BV(7 - i);
			}
			while(*(dht_port.pin) & BV(dht_port.input));
		}
		bits[j] = result;
	}
	
	*(dht_port.ddr) |= BV(dht_port.input);
	*(dht_port.port) |= BV(dht_port.input);
	_delay_ms(100);
	
	if (bits[0] + bits[1] + bits[2] + bits[3] == bits[4]) {
		if (select <= 3 && select >= 0) {
			return bits[select];	
		}
	}
	
	return DHT11_ERROR;
}