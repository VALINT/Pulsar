/*
 * _1802_One_Wire_HD44780.cpp
 *
 * Created: 12.02.2018 13:03:13
 *  Author: VAL
 */ 

#define F_CPU 8000000Ul


#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "PulsarHD44780.h"

char line[35];
/*
char sec = 00,min = 00,hour = 00,data,data1,a;
uint8_t x;
char line[15];

int numb0[6][8] = {
{0x1F,0x1F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F},
{0x1F,0x1F,0x00,0x00,0x00,0x00,0x1F,0x1F},
{0x00,0x00,0x0D,0x1F,0x1F,0x1F,0x0D,0x00},
{0x00,0x00,0x11,0x0A,0x04,0x0A,0x11,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};

void initial_microkontroller()			//функція ініціалізації мікроконтролера
{
	//	Налаштування портів
	//-------------------------------------------------------------------------------------------------------------------------------------
	DDRD = 0b00000001;
	PORTD = 0x11111110;
	//  Налаштування таймерів
	//---------------------------------------------------------------------------------------------------------------------------------------
	ASSR = (1<<AS2);								//Переведення таймера 2 в асинхронний режим, для тактування від зовнішнього кварцу на 32,768 КГц
	TCNT2 = 0x00;
	OCR2 = 0x00;
	TCCR2 = (1<<CS21);								//Налаштування без попереднього розділення частоти для таймера 2.
	TCCR0 = (1<<CS02);								//Налаштування без попереднього розділення частоти для таймера 0.
	TIMSK = (1<<TOIE2)|(1<<TOIE0);					//Дозвіл на переривання від таймерів 2 і 0.

	sei();
}


void print (uint8_t numb, uint8_t coordinat)
{
	switch (numb)
	{
		case 0:
		LCD_GotoXY(0,coordinat);
		LCD_SentChar(255);
		LCD_SentChar(0);
		LCD_SentChar(255);
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(255);
		LCD_SentChar(1);
		LCD_SentChar(255);
		break;
		
		case 1:
		LCD_GotoXY(0,coordinat);
		LCD_SentChar(5);
		LCD_SentChar(255);
		LCD_SentChar(5);
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(5);
		LCD_SentChar(255);
		LCD_SentChar(5);
		break;
		
		case 2:
		LCD_GotoXY(0,coordinat);
		LCD_SentChar(0);
		LCD_SentChar(0);
		LCD_SentChar(255);
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(255);
		LCD_SentChar(2);
		LCD_SentChar(2);
		break;
		
		case 3:
		LCD_GotoXY(0,coordinat);
		LCD_SentChar(2);
		LCD_SentChar(2);
		LCD_SentChar(255);
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(1);
		LCD_SentChar(1);
		LCD_SentChar(255);
		break;
		
		case 4:
		LCD_GotoXY(0,coordinat);
		LCD_SentChar(255);
		LCD_SentChar(1);
		LCD_SentChar(255);
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(5);
		LCD_SentChar(5);
		LCD_SentChar(255);
		break;
		
		case 5:
		LCD_GotoXY(0,coordinat);
		LCD_SentChar(255);
		LCD_SentChar(2);
		LCD_SentChar(2);
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(1);
		LCD_SentChar(1);
		LCD_SentChar(255);
		break;
		
		case 6:
		LCD_GotoXY(0,coordinat);
		LCD_SentChar(255);
		LCD_SentChar(2);
		LCD_SentChar(2);
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(255);
		LCD_SentChar(1);
		LCD_SentChar(255);
		break;
		
		case 7:
		LCD_GotoXY(0,coordinat);
		LCD_SentChar(0);
		LCD_SentChar(0);
		LCD_SentChar(255);
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(5);
		LCD_SentChar(5);
		LCD_SentChar(255);
		break;
		
		case 8:
		LCD_GotoXY(0,coordinat);
		LCD_SentChar(255);
		LCD_SentChar(2);
		LCD_SentChar(255);
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(255);
		LCD_SentChar(1);
		LCD_SentChar(255);
		break;
		
		case 9:
		LCD_GotoXY(0,coordinat);
		LCD_SentChar(255);
		LCD_SentChar(2);
		LCD_SentChar(255);
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(1);
		LCD_SentChar(1);
		LCD_SentChar(255);
		break;
		
		case 10:
		LCD_GotoXY(1, coordinat);
		LCD_SentChar(5);
		LCD_SentChar(5);
		LCD_SentChar(5);
		LCD_GotoXY(coordinat,1);
		LCD_SentChar(5);
		LCD_SentChar(5);
		LCD_SentChar(5);
		break;
	}
}

ISR (TIMER2_OVF_vect)				//Обробник переривання таймера 2.
{
	x++;
	if(x > 15)
	{
		x = 0;
		sec++;
	}
	if(sec > 59)
	{
		sec = 0;
		min++;
	}
	if(min > 59)
	{
		min=0;
		hour++;
	}
	if(hour > 23)
	hour=0;
	
	if(x%5 == 0)
	{
		if(PIND & (1 << PD1))
		{
			min ++;
			//_delay_ms(500);
		}
		if(PIND & (1 << PD2))
		{
			hour ++;
			//_delay_ms(500);
		}
		if(PIND & (1 << PD3))
		{
			sec = 0;
			//_delay_ms(500);
		}
	}
}

ISR (TIMER0_OVF_vect)
{
	if (a == 0)
	{
		a++;
		LCD_Initial();
		_delay_ms(1);
		LCD_SentByte(0x40,COMMAND);
		for (uint8_t i=0; i < 6; i++)
		{
			for(uint8_t j = 0; j < 8; j++)
			{
				data = numb0[i][j];
				LCD_SentByte(data,DATA);
			}
		}
	}
	
	uint8_t hour_high,hour_low,min_high,min_low;
	
	hour_high = hour/10;
	hour_low = hour%10;
	min_high = min/10;
	min_low = min%10;

	if(hour_high > 0)
	print(hour_high,0);
	else print(10,0);
	print(hour_low,4);
	
	print(min_high,8);
	print(min_low,12);
	
	LCD_GotoXY(0, 16);
	if(sec < 10) sprintf(line,"0%d",(sec));
	else sprintf(line,"%d",(sec));
	LCD_SentLine(line,2);
	
	if (x == 8)
	{
		LCD_GotoXY(0, 7);
		LCD_SentChar(4);
		LCD_GotoXY(1, 7);
		LCD_SentChar(4);
	}
	if (x == 0)
	{
		LCD_GotoXY(0, 7);
		LCD_SentChar(5);
		LCD_GotoXY(1, 7);
		LCD_SentChar(5);
	}
}

int main(void)
{
	initial_microkontroller();
	_delay_ms(100);
	LCD_Initial();
	_delay_ms(100);
	LCD_Clear();
	_delay_ms(100);
	LCD_SentByte(0x40,COMMAND);
	for (int i=0; i < 6; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			data = numb0[i][j];
			LCD_SentByte(data,DATA);
		}
	}
	
	while(1)
	{
		
		
	}
}*/

int main(void)
{
	
	DDRD = 0xff;
	LCD_Initial();
	_delay_ms(100);
	LCD_GotoXY(0,0);
	sprintf(line, "sample text SAMPLE TEXT sample text");
	while(1)
	{
		LCD_GotoXY(0,0);
		_delay_ms(1);
		LCD_Cursor(CURSOR_BLINK);
		_delay_ms(2000);
		
		LCD_GotoXY(0,0);
		_delay_ms(1);
		LCD_Cursor(CURSOR_ON);
		_delay_ms(2000);
		
		LCD_GotoXY(0,0);
		_delay_ms(1);
		LCD_Cursor(CURSOR_OFF);
		_delay_ms(2000);
		
		LCD_GotoXY(0,0);
		
		for(uint8_t i = 0; i < 13; i++)
		{
			LCD_Scroll(1, SCROLL_LEFT);
			_delay_ms(330);
		}
		
		LCD_Clear();
		LCD_GotoXY(0,0);
		LCD_SentLine(line, 29);
		_delay_ms(500);
		while(1)
		{
		for(uint8_t i = 0; i < 19; i++)
		{
			LCD_Scroll(1, SCROLL_LEFT);
			_delay_ms(330);
		}
		_delay_ms(1000);
		for(uint8_t i = 0; i < 19; i++)
		{
			LCD_Scroll(1, SCROLL_RIGHT);
			_delay_ms(330);
		}
		}		
		LCD_State(DISPLAY_OFF);
		_delay_ms(1000);
		LCD_State(DISPLAY_ON);
		_delay_ms(2000);
		LCD_Clear();
	}
}
//*/

/*int main(void)
 {
	 DDRD = 0xff;
	 initial_LCD();
	 SentByte(0xff);
	 initial_LCD();
	 _delay_ms(100);
	 initial_LCD();
	 _delay_ms(100);
	 initial_LCD();
	 _delay_ms(100);
	 sent_coordinats(0,0);
	 while(1)
	 {
		 sent_char(0b01001111);
		 _delay_ms(200);
		 sent_char(0b10111011);
		 _delay_ms(200);
		 sent_char(0b01100101);
		 _delay_ms(200);
		 sent_char(0b10110100);
		 _delay_ms(200);
		 
		 sent_char(0b00100000);
		 _delay_ms(200);
		 
		  sent_char(0b10111101);
		  _delay_ms(200);
		   sent_char(0b01100101);
		   _delay_ms(200);
		 
		  sent_char(0b00100000);
		  _delay_ms(200);
		 
		 sent_char(0b10101000);
		 _delay_ms(200);
		 sent_char(0b01001001);
		 _delay_ms(200);
		 sent_char(0b11100000);
		 _delay_ms(200);
		 sent_char(0b01001111);
		 _delay_ms(200);
		 sent_char(0b01010000);
		 _delay_ms(5000);
		 
		 ClearLCD();
		 
		 sent_coordinats(0,0);
		 
		 sent_char(33);
		 _delay_ms(200);
		 sent_char(34);
		 _delay_ms(200);
		 sent_char(35);
		 _delay_ms(200);
		 sent_char(36);
		 _delay_ms(200);
		 
		 sent_char(37);
		 _delay_ms(200);
		 
		 sent_char(38);
		 _delay_ms(200);
		 sent_char(39);
		 _delay_ms(200);
		 sent_char(40);
		 _delay_ms(200);
		 sent_char(41);
		 _delay_ms(200);
		 sent_char(42);
		 _delay_ms(5000);
		 
		 ClearLCD();
		 


	 
	 sent_byte(0x40,COMAND);			//Выбираем адрес
	 for (uint8_t i = 0;i < 8;i++)		//Цикл, передающий символ в память контроллера
	 {
		 sent_byte(Symbol[i],DATA);
	 }
	 sent_coordinats(0,0);			//Выставляем положение курсора
	 sent_char(0);
	 
	 _delay_ms(5000);
		 
		 while(1)
		 {
			 sent_coordinats(0,0);
			 _delay_ms(10);
			 sent_char(x);
			 x++;
			 _delay_ms(500);
		 }		 
	 }
 }*/