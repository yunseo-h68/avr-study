/*
 * dht11.h
 *
 * Created: 2020-12-09 오전 10:32:05
 *  Author: Yunseo Hwang
 */ 

#ifndef DHT11_H_
#define DHT11_H_

#define DHT11_ERROR 255
#define TEMPERATURE 2
#define HUMIDITY    0

struct dht11_port {
	volatile uint8_t *ddr;
	volatile uint8_t *port;
	volatile uint8_t *pin;
	volatile uint8_t input;
};

uint8_t dht11_getdata(uint8_t select);
void set_dht11_port(struct dht11_port port);

#endif /* DHT11_H_ */