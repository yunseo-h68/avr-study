/*
 * w06p02_joystic.c
 *
 * Created: 2020-07-29 오전 11:04:51
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "adc.h"
#include "lcd1602a_h68.h"
#include "uart.h"

void init_lcd();
void send_data(int data[]);

int main(void)
{
	int data[4] = {0,};
	char buffer[20];
	int ADC_value;
	
	ADC_init();
	init_lcd();
	uart_init(BAUDRATE(9600));
	
    while (1) 
    {
		ADC_value = ADC_read(0);
		data[0] = (ADC_value & 0xFF00) >> 8;
		data[1] = ADC_value & 0x00FF;
		sprintf(buffer, "X-%04d", ADC_value);
		lcd_move(0,0);
		lcd_puts(buffer);
		
		ADC_value = ADC_read(1);
		data[2] = (ADC_value & 0xFF00) >> 8;
		data[3] = ADC_value & 0x00FF;
		sprintf(buffer, "Y-%04d", ADC_value);
		lcd_move(0,1);
		lcd_puts(buffer);
		
		send_data(data);
    }
}

void init_lcd()
{
	struct lcd1602a_port port = {&DDRC, &DDRC, &PORTC, &PORTC};
	set_lcd_bit(4);
	set_lcd_port(port);
	lcd_init(LCD_ROWS_MAX, LCD_COLS_MAX);
}

void send_data(int data[])
{
	uart_write(0x02);
	uart_write(data[0]);
	uart_write(data[1]);
	uart_write(data[2]);
	uart_write(data[3]);
	uart_write(0x03);
}