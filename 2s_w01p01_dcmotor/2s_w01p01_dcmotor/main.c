/*
 * 2s_w01p01_dcmotor.c
 *
 * Created: 2020-09-09 오전 9:04:49
 * Author : user
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "lcd1602a_h68.h"
#include "uart.h"

#define BV(n) (1 << n)

#define MOTOR_STOP 0x00
#define MOTOR_COROTATION 0x02
#define MOTOR_REVERSAL 0x01

#define BUTTON_UP BV(3)
#define BUTTON_DOWN BV(2)
#define BUTTON_STOP BV(4)

#define MOTOR_UP (~PINA & BUTTON_UP)
#define MOTOR_DOWN (~PINA & BUTTON_DOWN)
#define MOTOR_ZERO (~PINA & BUTTON_STOP)

#define MOTOR_SPEED_STOP 0
#define MOTOR_SPEED_UP 1
#define MOTOR_SPEED_DOWN 2

#define MOTOR_MIN 80
#define MOTOR_MAX 100
#define MOTOR_SPEED_LEVEL 4

#define LCD_MOTOR_SPEED(n) ((n - 80)*5)

void timer_init();
void delay_with_button(unsigned int t);
void show_motor_speed_led();
void control_motor_speed(int control);
void print_lcd_motor_speed();
void processing_data();

int motor_speed = 0;
volatile int USART0_rx_end = 0;
volatile int USART0_data = 0;

void processing_data()
{
	switch(USART0_data) {
		case 0x01:
			motor_speed = 0;
			break;
		case 0x02:
			motor_speed = 84;
			break;
		case 0x03:
			motor_speed = 88;
			break;
		case 0x04:
			motor_speed = 92;
			break;
		case 0x05:
			motor_speed = 96;
			break;
		case 0x06:
			motor_speed = 100;
			break;
	}
	OCR1A = motor_speed;
	USART0_rx_end = 0;
}

int main(void)
{
	struct lcd1602a_port port = {&DDRC, &DDRC, &PORTC, &PORTC};
	
	motor_speed = MOTOR_MIN;
	DDRA = 0x03;
	PORTA = 0x00;
	DDRD = 0xFF;
	PORTD = 0x00;
	DDRB = 0xFF;
	PORTB = 0x00;
	
	set_lcd_bit(4);
	set_lcd_port(port);
	lcd_init(LCD_ROWS_MAX - 6, LCD_COLS_MAX);
	
	sei();
	timer_init();
	uart_init(BAUDRATE(9600));
	show_motor_speed_led();
	
    while (1) 
    {
		if (USART0_rx_end) {
			processing_data();
		}
		
		PORTA = MOTOR_STOP;
		delay_with_button(1000);
		PORTA = MOTOR_COROTATION;
		delay_with_button(5000);
		PORTA = MOTOR_STOP;
		delay_with_button(1000);
		PORTA = MOTOR_REVERSAL;
		delay_with_button(5000);
    }
}

void timer_init()
{
	TCCR1A = 0x82;
	TCCR1B = 0x1B;
	ICR1 = MOTOR_MAX;
	OCR1A = 0;
	TCNT1 = 0x00;
	lcd_clear();
	lcd_putc(LCD_MOTOR_SPEED(MOTOR_MIN) / 100 + '0');
	lcd_putc((LCD_MOTOR_SPEED(MOTOR_MIN) / 10) % 10 + '0');
	lcd_putc(LCD_MOTOR_SPEED(MOTOR_MIN) % 10 + '0');
}

void control_motor_speed(int control)
{
	switch(control) {
		case MOTOR_SPEED_STOP:
			motor_speed = 0;
			break;
		case MOTOR_SPEED_UP:
			if (motor_speed < MOTOR_MIN) {
				motor_speed = MOTOR_MIN;
			}
			motor_speed = motor_speed + MOTOR_SPEED_LEVEL > MOTOR_MAX ? MOTOR_MIN : motor_speed + MOTOR_SPEED_LEVEL;
			OCR1A = motor_speed;
			break;
		case MOTOR_SPEED_DOWN:
			if (motor_speed < MOTOR_MIN) {
				motor_speed = MOTOR_MIN;
			}
			motor_speed = motor_speed - MOTOR_SPEED_LEVEL < MOTOR_MIN ? MOTOR_MAX : motor_speed - MOTOR_SPEED_LEVEL;
			OCR1A = motor_speed;
			break;
	}
	if (motor_speed <= MOTOR_MIN) {
		OCR1A = 0;
	}
}

void print_lcd_motor_speed()
{
	lcd_clear();
	if (motor_speed < MOTOR_MIN) {
		lcd_puts("000");
		return;	
	}
	lcd_putc(LCD_MOTOR_SPEED(motor_speed) / 100 + '0');
	lcd_putc((LCD_MOTOR_SPEED(motor_speed) / 10) % 10 + '0');
	lcd_putc((LCD_MOTOR_SPEED(motor_speed)) % 10 + '0');
}

void delay_with_button(unsigned int t)
{
	int i;
	for (i = 0; i < t; i++) {
		_delay_ms(1);
		
		if (USART0_rx_end) {
			processing_data();
			print_lcd_motor_speed();
			show_motor_speed_led();
		}
		
		if (MOTOR_UP) {
			_delay_ms(400);
			control_motor_speed(MOTOR_SPEED_UP);
			print_lcd_motor_speed();
			show_motor_speed_led();
		} else if (MOTOR_DOWN) {
			_delay_ms(400);
			control_motor_speed(MOTOR_SPEED_DOWN);
			print_lcd_motor_speed();
			show_motor_speed_led();
		} else if (MOTOR_ZERO) {
			_delay_ms(400);
			control_motor_speed(MOTOR_SPEED_STOP);
			print_lcd_motor_speed();
			show_motor_speed_led();
		}
	}
}

void show_motor_speed_led() 
{
	PORTD = BV(0);
	if (LCD_MOTOR_SPEED(motor_speed) >= 20) PORTD = BV(1);
	if (LCD_MOTOR_SPEED(motor_speed) >= 40) PORTD = BV(2);
	if (LCD_MOTOR_SPEED(motor_speed) >= 60) PORTD = BV(3);
	if (LCD_MOTOR_SPEED(motor_speed) >= 80) PORTD = BV(4);
	if (LCD_MOTOR_SPEED(motor_speed) >= 100) PORTD = BV(5);
}

ISR(USART0_RX_vect)
{
	unsigned char buf = UDR0;
	if (buf == 0x01 || buf == 0x02 || buf == 0x03 || buf == 0x04 || buf == 0x05 || buf == 0x06) {
		USART0_rx_end = 1;
		USART0_data = buf;
		uart_write('O');
		uart_write('K');
		uart_write(' ');
	}
}