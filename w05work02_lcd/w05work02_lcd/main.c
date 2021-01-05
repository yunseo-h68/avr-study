/*
 * w05p01_lcd.c
 *
 * Created: 2020-06-24 오전 9:31:22
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd1602a_h68.h"

int main(void)
{
	struct lcd1602a_port port = {&DDRC, &DDRC, &PORTC, &PORTC};
	int time_lcd = 0;
	int dir = 0;
	
	set_lcd_bit(4);
	set_lcd_port(port);
	lcd_init(LCD_ROWS_MAX, LCD_COLS_MAX);
	
	lcd_move(6, 0);
	lcd_puts("TIME");
	
	while (1) {
		if (~PINE & (1 << 0)) {
			dir = !dir;
			_delay_ms(400);
		}
		if (~PINE & (1 << 1)) {
			time_lcd = 0;
			_delay_ms(400);
		}
		
		time_lcd += dir;
		
		lcd_move(4, 1);
		lcd_putc('0' + time_lcd / 100000);
		lcd_putc('0' + (time_lcd / 10000) % 10);
		lcd_putc(':');
		lcd_putc('0' + (time_lcd / 1000) % 10);
		lcd_putc('0' + (time_lcd / 100) % 10);
		lcd_putc(':');
		lcd_putc('0' + (time_lcd / 10) % 10);
		lcd_putc('0' + time_lcd % 10);
		
		_delay_ms(10);
	}
}