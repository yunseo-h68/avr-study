/*
 * 2s_w07p01_laser.c
 *
 * Created: 2020-11-11 오전 9:11:05
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#include "uart.h"

#define BV(n) (1 << n)
#define ON(n) PORTB |= BV(n);
#define OFF(n) PORTB &= ~BV(n);
#define LED_ON(n) {PORTB &= 0x01; ON(n);}
#define LED_OFF {PORTB &= 0x01;}

#define LASER 0
#define RED   1
#define GREEN 2
#define BLUE  3

void send_led(int color);

int main(void)
{
	int cds = 0;
	
	DDRB = 0xFF;
	DDRA = 0x00;
	PORTB = 0x00;
	ADC_init();
	uart_init(BAUDRATE(9600));
	
	// 전원 인가 시 녹색 LED ON
	LED_ON(GREEN);
	send_led(GREEN);
	_delay_ms(3000);
	
	// LASER ON
	ON(LASER);
	LED_ON(BLUE);
	send_led(BLUE);
	
	_delay_ms(500);
	
    while (1) 
    {
		cds = ADC_read(0);
		if (cds > 80) {
			LED_ON(RED);
			send_led(RED);
		} else {
			LED_ON(BLUE);
			send_led(BLUE);
		}
    }
}

void send_led(int color)
{
	uart_write(color);
}