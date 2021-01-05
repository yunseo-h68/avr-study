/*
 * w05work03_lcd.c
 *
 * Created: 2020-06-24 오전 9:31:22
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

//  PC7 PC6 PC5 PC4 PC3 PC2 PC1 PC0
//  DB7 DB6 DB5 DB4      E   RW  RS

void COMMAND(unsigned char byte);	// COMMAND 함수 선언
void DATA(unsigned char byte);		// DATA 함수 선언
void LCD_INIT(void);			// LCD_INIT 함수 선언
void MOVE(unsigned char x, unsigned char y); 
void DATAS(const char* str);

int main(void)
{
	int num[4] = {0,0,0,0};
	int x = 0;
	
	DDRC = 0xff;
	DDRE = 0x00;
	PORTC = 0x00;

	LCD_INIT();
	
	DATAS("0000");
	MOVE(0, 0);
	
	COMMAND(0b00001110);
	
	while (1) {
		if (~PINE & (1 << 0)) {
			MOVE(x, 0);
			num[x]++;
			if (num[x] == 10)
				num[x] = 0;
			DATA('0' + num[x]);
			MOVE(x, 0);
			_delay_ms(400);
		}
		else if (~PINE & (1 << 1)) {
			MOVE(x, 0);
			num[x]--;
			if (num[x] == -1)
				num[x] = 9;
			DATA('0' + num[x]);
			MOVE(x, 0);
			_delay_ms(400);
		}
		else if (~PINE & (1 << 2)) {
			x++;
			if (x == 4)
				x = 0;
			MOVE(x, 0);
			_delay_ms(400);
		}
		else if (~PINE & (1 << 3)) {
			x--;
			if (x == -1)
				x = 3;
			MOVE(x, 0);
			_delay_ms(400);
		}
	}
}

void DATA(unsigned char byte)
{
	_delay_ms(2);

	PORTC=byte&0xf0;// 상위4비트 명령어 쓰기
	PORTC |= 0b00000001;// RS = 1, 데이터 설정
	PORTC &= 0b11111101;// RW = 0, 쓰기 설정
	_delay_us(1);
	PORTC|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTC&=0b11111011;// E = 0

	PORTC=(byte<<4)&0xf0;// 하위4비트 데이터 쓰기
	PORTC |= 0b00000001;// RS = 1, 데이터 설정
	PORTC &= 0b11111101;// RW = 0, 쓰기 설정
	_delay_us(1);
	PORTC|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTC&=0b11111011;// E = 0
}

void COMMAND(unsigned char byte)
{
	_delay_ms(2);

	PORTC=byte&0xf0;// 상위4비트 명령어 쓰기
	PORTC&=0b11111100;  // RS = 0, RW = 0, 명령어를 쓰도록 설정
	_delay_us(1);
	PORTC|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTC&=0b11111011;// E = 0

	PORTC=(byte<<4)&0xf0;// 하위4비트 명령어 쓰기
	PORTC&=0b11111100;// RS = 0, RW = 0, 명령어를 쓰도록 설정
	_delay_us(1);
	PORTC|=0b00000100; // E = 1, lcd 동작
	_delay_us(1);
	PORTC&=0b11111011;// E = 0
}

void DATAS(const char* str)
{
	int i = 0;
	for(i = 0; str[i]; i++) {
		DATA(str[i]);
	}
}

void LCD_INIT(void)
{
	_delay_ms(30);	// 전원 투입 후 30ms 이상 지연
	
	//Function set
	COMMAND(0b00101000);
	// 인터페이스(DL)=0(4bit), 라인(N)=1(2라인), 폰트(F)=0(5*8 dot)
	_delay_us(39);	// 39us 이상 지연

	//Display ON/OFF Control
	COMMAND(0b00001100);
	// 화면 표시(D)=1(on), 커서(C)=0(off), 블링크(B)=0(off)
	_delay_us(39);	// 39us 이상 지연

	//Clear Display
	COMMAND(0b00000001);
	// 화면을 클리어하고 , 커서가 홈위치인 0번지로 돌아감.
	_delay_ms(1.53);	// 1.53ms 이상 지연

	//Entry Mode Set
	COMMAND(0b00000110);
	// 커서방향(I/D)=1(address증가), 표시이동(S)=0(이동하지 않음)
}

void MOVE(unsigned char x, unsigned char y)
{
	int i;
	
	COMMAND(0b00000010);
	
	for (i = 0; i < x + y * 40; i++) {
		COMMAND(0b00010100);
		_delay_us(39);
	}
}

