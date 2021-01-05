/*
 * 2s_w02p01_ultrasonic.c
 *
 * Created: 2020-09-23 오전 9:49:37
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd1602a_h68.h"
#include "uart.h"

#define BV(n) (1 << n)
#define SOUND_VELOCITY 340UL
#define TRIG 6
#define ECHO 7

void ultrasonic_set();
unsigned int ultrasonic_distance();
void lcd_print_distance(unsigned int distance);
void uart_write_distance(unsigned int distance);

int main(void)
{
	unsigned int distance = 0;
	struct lcd1602a_port port = {&DDRC, &DDRC, &PORTC, &PORTC};
	
	DDRB = 0xFF;
	DDRD = 0xFF;
	
	set_lcd_bit(4);
	set_lcd_port(port);
	lcd_init(LCD_ROWS_MAX, LCD_COLS_MAX);
	
	uart_init(BAUDRATE(9600));
	ultrasonic_set();
	
    while (1) 
    {
		distance = ultrasonic_distance();
		lcd_print_distance(distance);
		if (distance <= 30) PORTD = BV(0);
		else if (distance <= 70) PORTD = BV(1);
		else if (distance <= 100) PORTD = BV(2);
		else if (distance <= 130) PORTD = BV(3);
		else if (distance <= 200) PORTD = BV(4);
		else PORTD = BV(5);
		
		uart_write_distance(distance);
		
		_delay_ms(1000);
    }
}

void ultrasonic_set()
{
	DDRE = (DDRE | (1 << TRIG)) & ~(1 << ECHO);
}

unsigned int ultrasonic_distance()
{
	unsigned int distance;
	TCCR1B = 0x03;
	PORTE &= ~(1<<TRIG);
	_delay_us(10);
	PORTE |= (1<<TRIG);
	_delay_us(10);
	PORTE &= ~(1<<TRIG);
	while(!(PINE & (1 << ECHO)));
	TCNT1 = 0x0000;
	while(PINE & (1 << ECHO));
	TCCR1B = 0x00;
	distance = SOUND_VELOCITY * (TCNT1 * 4 / 2) / 1000;
	return distance;
}

void lcd_print_distance(unsigned int distance)
{
	lcd_clear();
	lcd_putc(distance / 1000 + '0');
	lcd_putc((distance / 100) % 10 + '0');
	lcd_putc((distance / 10) % 10 + '0');
	lcd_putc(distance % 10 + '0');
	lcd_puts("mm");
}

void uart_write_distance(unsigned int distance)
{
	uart_write(distance / 1000 + '0');
	uart_write((distance / 100) % 10 + '0');
	uart_write((distance / 10) % 10 + '0');
	uart_write(distance % 10 + '0');
	uart_write(' ');
}
