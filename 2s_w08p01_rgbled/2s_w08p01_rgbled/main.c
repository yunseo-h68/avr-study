/*
 * 2s_w08p01_rgbled.c
 *
 * Created: 2020-11-25 오전 9:01:40
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "dht11.h"
#include "rgb_led.h"

volatile int USART0_rx_end = 0;
volatile int USART0_data_index = 0;
volatile int USART0_data[100] = {0,};

ISR(USART0_RX_vect)
{	
	unsigned char buf = UDR0;
	if (buf == 0x02)
	{
		USART0_data_index = 1;
		return;
	}
	if (USART0_data_index)
	{
		if (buf == 0x03)
		{
			USART0_data[USART0_data_index - 1] = 0;
			USART0_data_index = 0;
			USART0_rx_end = 1;
			return;
		}
		USART0_data[USART0_data_index - 1] = buf;
		USART0_data_index++;
	}
}

int main(void)
{
	sei();
	DDRB = 0xFF;
	DDRA = 0x01;
	
	struct dht11_port dht_port = {&DDRF, &PORTF, &PINF, PORTF1};
	set_dht11_port(dht_port);
	
	uart_init(BAUDRATE(9600));
	timer_init();
    while (1) 
    {
		if (USART0_rx_end) {
			switch(USART0_data[0]) {
				case 0x04:
					set_color(USART0_data[1], USART0_data[2], USART0_data[3]);
					uart_write(USART0_data[1]);
					uart_write(USART0_data[2]);
					uart_write(USART0_data[3]);
					break;
				case 0x05:
					uint8_t buf = 0;
					buf = dht11_getdata(TEMPERATURE);
					uart_write(buf);
					buf = dht11_getdata(HUMIDITY);
					uart_write(buf);
					break;
			}
			USART0_rx_end = 0;
		}
    }
}

