/*
 * atmega128_uart.c
 *
 * Created: 2020-07-01 오전 10:18:48
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#define BAUDRATE(x) ((F_CPU / 16 / x) -1)
#define BV(n) (1 < n)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

volatile int USART1_len = 0;
volatile char Rx_Buffer[30] = {0,};
volatile uint8_t USART1_rx_end = 0;

void uart_init(unsigned int baud)
{
	UBRR0H = (unsigned char)(baud >> 8);
	UBRR0L = (unsigned char)baud;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
}

void uart_write(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void processing_data()
{
	if (!strcmp(Rx_Buffer, "\x02L1ON\x03")) PORTF |= BV(5);            
	else if (!strcmp(Rx_Buffer, "\x02L1OFF\x03")) PORTF &= ~BV(5);
	else if (!strcmp(Rx_Buffer, "\x02L2ON\x03")) PORTF |= ~BV(4);
	else if (!strcmp(Rx_Buffer, "\x02L2OFF\x03")) PORTF &= ~BV(4);
	else if (!strcmp(Rx_Buffer, "\x02L3ON\x03")) PORTF |= ~BV(3);
	else if (!strcmp(Rx_Buffer, "\x02L3OFF\x03")) PORTF &= ~BV(3);
	USART1_rx_end = 0;
}

int main(void)
{
	DDRF = 0xFF;
	PORTF = 0x00;
	uart_init(BAUDRATE(115200));
	sei();
    while (1) 
	{
		if (USART1_rx_end) {
			processing_data();
		}
	}
}

ISR(USART0_RX_vect)
{	
	static uint8_t usart1_chk = 0;
	unsigned char buf = UDR0;
	
	USART1_rx_end = 0;
	
	switch(usart1_chk)
	{
		case 0:
			if (buf == 0x02) {
				Rx_Buffer[USART1_len] = buf;
				USART1_len++;
				usart1_chk = 1;
			} else usart1_chk = 0;
			break;
		case 1:
			Rx_Buffer[USART1_len] = buf;
			USART1_len++;
			
			if (buf == 0x03) {
				USART1_rx_end = 1;
				usart1_chk = 0;
			}
			break;
		default:
			usart1_chk = 0;
			break;
	}
}