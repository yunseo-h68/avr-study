/*
 * 2s_w03p01_dht11.c
 *
 * Created: 2020-10-07 오전 9:25:34
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd1602a_h68.h"
#include "uart.h"

#define DHT11_DDR DDRA
#define DHT11_PORT PORTA
#define DHT11_PIN PINA
#define DHT11_DATA_PIN 0

#define BV(n) (1 << n)

void dht11_request();
void dht11_response();
uint8_t dht11_receive_data();
void lcd_print_dht11(uint8_t i_temp, uint8_t d_temp, uint8_t i_rh, uint8_t d_rh);
void send_data(uint8_t i_temp, uint8_t d_temp, uint8_t i_rh, uint8_t d_rh);

int main(void)
{
	uint8_t i_rh, d_rh, i_temp, d_temp, check_sum;
	struct lcd1602a_port port = {&DDRC, &DDRC, &PORTC, &PORTC};

	set_lcd_bit(4);
	set_lcd_port(port);
	lcd_init(LCD_ROWS_MAX, LCD_COLS_MAX);
	uart_init(BAUDRATE(9600));
	
    while (1) 
    {
		dht11_request();
		dht11_response();
		i_rh = dht11_receive_data();
		d_rh = dht11_receive_data();
		i_temp = dht11_receive_data();
		d_temp = dht11_receive_data();
		check_sum = dht11_receive_data();
		
		if ((i_rh + d_rh + i_temp + d_temp) != check_sum) {
			lcd_clear();
			lcd_move(0, 0);
			lcd_puts("ERROR");
		} else {
			lcd_print_dht11(i_temp, d_temp, i_rh, d_rh);
			send_data(i_temp, d_temp, i_rh, d_rh);
		}
		_delay_ms(3000);
    }
}

void dht11_request()
{
	DHT11_DDR |= BV(DHT11_DATA_PIN);
	DHT11_PORT &= ~BV(DHT11_DATA_PIN);
	_delay_ms(20);
	DHT11_PORT |= BV(DHT11_DATA_PIN);
}

void dht11_response()
{
	DHT11_DDR &= ~BV(DHT11_DATA_PIN);
	while (DHT11_PIN & BV(DHT11_DATA_PIN));
	while ((DHT11_PIN & BV(DHT11_DATA_PIN)) == 0);
	while (DHT11_PIN & BV(DHT11_DATA_PIN));	
}

uint8_t dht11_receive_data()
{
	uint8_t ret = 0;
	
	for (int q = 0; q < 8; q++)
	{
		while ((DHT11_PIN & BV(DHT11_DATA_PIN)) == 0);
		_delay_us(30);
		if (DHT11_PIN & BV(DHT11_DATA_PIN)) {
			ret = (ret << 1) | 0x01;
		} else {
			ret = ret << 1;
		}
		while (DHT11_PIN & BV(DHT11_DATA_PIN));
	}
	return ret;
}

void lcd_print_dht11(uint8_t i_temp, uint8_t d_temp, uint8_t i_rh, uint8_t d_rh)
{
	char data[5] = {0,};
		
	lcd_clear();
	lcd_move(0, 0);
	
	lcd_puts("TEMP : ");
	itoa(i_temp, data, 10);
	lcd_puts(data);
	lcd_putc('.');
	itoa(d_temp, data, 10);
	lcd_puts(data);
	lcd_putc(0xDF);
	lcd_putc('C');
	
	lcd_move(0, 1);
	lcd_puts("Humidity : ");
	itoa(i_rh, data, 10);
	lcd_puts(data);
	lcd_putc('.');
	itoa(d_rh, data, 10);
	lcd_puts(data);
	lcd_putc('%');
}

void send_data(uint8_t i_temp, uint8_t d_temp, uint8_t i_rh, uint8_t d_rh)
{
	//Protocol :  <0x02>T00.0H00.0C000<0x03>
	uart_write(0x02);
	uart_write('T');
	uart_write(i_temp / 10 + '0');
	uart_write(i_temp % 10 + '0');
	uart_write('.');
	uart_write(d_temp % 10 + '0');
	
	uart_write('H');
	uart_write(i_rh / 10 + '0');
	uart_write(i_rh % 10 + '0');
	uart_write('.');
	uart_write(d_rh % 10 + '0');
	
	uart_write('C');
	uart_write('0');
	uart_write('0');
	uart_write('0');
	uart_write(0x03);
	uart_write('\n');
}