/*
 * rgb_led.c
 *
 * Created: 2020-12-09 오전 11:00:42
 *  Author: Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define OCR1_R OCR1A
#define OCR1_G OCR1B
#define OCR1_B OCR1C

void timer_init()
{
	// Fast PWM 8bit
	// WGM13(0), WGM12(1), WGM11(0), WGM10(1)
	// COM1A1/COM1B1/COM1C1 : 1, COM1A0/COM1B0/COM1C0 : 0
	TCCR1A |= (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1) | (1<<COM1C1);
	TCCR1B |= (1<<WGM12) | (1<<CS11);
	set_color(0, 0, 0);
}

void set_color(int red, int green, int blue)
{
	OCR1_R = red   & 255;
	OCR1_G = green & 255;
	OCR1_B = blue  & 255;
}