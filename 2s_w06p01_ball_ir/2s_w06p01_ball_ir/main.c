/*
 * 2s_w06p01_ball_ir.c
 *
 * Created: 2020-11-04 오전 9:55:12
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

#define BALL_PIN 0
#define IR_PIN 1
#define BV(n) (1 << n)

void send_data(char ball_sw, char ir_sensor);

int main(void)
{	
	DDRA = 0x00;
	DDRB = 0xFF;
	DDRD = 0xFF;
	PORTB = 0xFF;
	PORTD = 0x00;
	uart_init(BAUDRATE(9600));
	
    while (1) 
    {
		if (~PINA & BV(BALL_PIN)) {
			_delay_ms(400);
			send_data(1, 0);
			PORTD |= BV(BALL_PIN);
			_delay_ms(100);
			PORTD &= ~BV(BALL_PIN);
		}
		
		if (~PINA & BV(IR_PIN)) {
			_delay_ms(400);
			send_data(0, 1);
			PORTD |= BV(IR_PIN);
			_delay_ms(100);
			PORTD &= ~BV(IR_PIN);
		}
    }
}

void send_data(char ball_sw, char ir_sensor)
{
	uart_write(0x02);
	uart_write(ball_sw);
	uart_write(ir_sensor);
	uart_write(0x03);
}