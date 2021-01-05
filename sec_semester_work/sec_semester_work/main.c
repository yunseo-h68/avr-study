/*
 * sec_semester_work.c
 *
 * Created: 2020-12-11 오후 6:49:57
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd1602a_h68.h"
#include "rgb_led.h"
#include "uart.h"
#include "adc.h"

#define BV(n) (1 << (n))

#define STOP_BUTTON_PIN  PORTD0
#define ALARM_BUZZER_PIN PORTD1
#define LASER_PIN        PORTD2
#define LCD_POWER_PIN    PORTD3

#define MOTOR_STOP       0x00
#define MOTOR_COROTATION 0x02
#define MOTOR_REVERSAL   0x01


#define CMD_SLEEP 0x04
#define CMD_RESET 0x05

#define RESPONSE_OK  1
#define RESPONSE_ERR 0

void initial();
int intrusion_detection();
void uart_send(uint8_t detected);
void uart_response(uint8_t is_ok);
void lcd_on();
void lcd_off();
void make_rainbow_rgb();
uint8_t do_detection(uint8_t before_detected);
uint8_t process_cmd();

volatile int USART0_rx_end = 0;
volatile int USART0_data_index = 0;
volatile int USART0_data[100] = {0,};

int main(void)
{
	int detected = 0;
	int power_state = 1;
	
	initial();
	
	while (1) 
    {	
		if (USART0_rx_end) {
			power_state = process_cmd();
			USART0_rx_end = 0;
		}
		
		if (!power_state) {
			continue;
		}
		
		if (!detected) {
			make_rainbow_rgb();
		}
		
		detected = do_detection(detected);
    }
}

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

void initial()
{
	lcd_on();
	lcd_puts("Initializing ... ");
	
	sei();
	ADC_init();
	uart_init(BAUDRATE(9600));
	rgb_init();
	
	DDRD = 0x00 | BV(ALARM_BUZZER_PIN) | BV(LASER_PIN);
	DDRD &= ~BV(STOP_BUTTON_PIN);
	PORTD =  0x00 | BV(LASER_PIN);
	
	_delay_ms(500);
	
	lcd_clear();
	lcd_puts("Wait...");
}

int intrusion_detection()
{
	int detected = 0;
	int cds = ADC_read(0);
	if (cds > 80 && (PORTD & BV(LASER_PIN))) {
		detected = 1;
	} else {
		detected = 0;
	}
	return detected;
}

void uart_send(uint8_t detected)
{
	uart_write(0x02);
	uart_write(detected + '0');
	uart_write(0x03);
}

void uart_response(uint8_t is_ok)
{
	uart_write(0x02);
	if (is_ok == RESPONSE_OK) {
		uart_write(0x50);
	} else {
		uart_write(0x60);	
	}
	uart_write(0x03);
}

void lcd_on()
{
	struct lcd1602a_port port = {&DDRC, &DDRC, &PORTC, &PORTC};
		
	DDRD |= BV(LCD_POWER_PIN);
	PORTD |= BV(LCD_POWER_PIN);
	_delay_ms(200);
	
	set_lcd_bit(4);
	set_lcd_port(port);
	lcd_init(LCD_ROWS_MAX, LCD_COLS_MAX);
	lcd_clear();
}

void lcd_off()
{
	lcd_clear();
	PORTD &= ~BV(LCD_POWER_PIN);
}

void make_rainbow_rgb()
{
	static uint8_t color = COLOR_RED;
	color++;
	if (color > COLOR_VIOLET) {
		color = COLOR_RED;
	}
	set_rainbow(color);
}

uint8_t do_detection(uint8_t before_detected)
{
	uint8_t detected = intrusion_detection();
	
	if (!detected && before_detected) {
		if (~PIND & BV(STOP_BUTTON_PIN)) {
			PORTD &= ~BV(ALARM_BUZZER_PIN);
			uart_send(0);
			
			lcd_clear();
			lcd_puts("Wait..");
			before_detected = 0;
			
			PORTA = MOTOR_REVERSAL;
			_delay_ms(500);
			PORTA = MOTOR_STOP;
		}
		return before_detected;
	}
	
	if (detected && !before_detected) {
		set_rainbow(COLOR_RED);
		
		lcd_clear();
		lcd_puts("Detected!!!");
		
		uart_send(1);
		PORTD |= BV(ALARM_BUZZER_PIN);
		
		PORTA = MOTOR_COROTATION;
		_delay_ms(500);
		PORTA = MOTOR_STOP;
	}
	
	return detected;
}

uint8_t process_cmd()
{
	switch(USART0_data[0]) {
		case CMD_SLEEP:
			rgb_off();
			lcd_off();
			PORTD &= ~BV(LASER_PIN);
			uart_response(RESPONSE_OK);
			return 0;
		case CMD_RESET:
			initial();
			PORTD |= BV(LASER_PIN);
			uart_response(RESPONSE_OK);
			return 1;
		default:
			lcd_clear();
			lcd_putc(USART0_data[0] + '0');
			uart_response(RESPONSE_ERR);
	}
	return 1;
}