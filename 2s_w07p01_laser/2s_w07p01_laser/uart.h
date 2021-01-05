/*
 * uart.h
 *
 * Created: 2020-07-29 오전 11:20:55
 *  Author: user
 */ 


#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU / 16 / x) - 1)

#include <avr/io.h>
#include <util/delay.h>

void uart_init(unsigned int baud);
void uart_write(unsigned char data);
unsigned char uart_read();

#endif /* UART_H_ */