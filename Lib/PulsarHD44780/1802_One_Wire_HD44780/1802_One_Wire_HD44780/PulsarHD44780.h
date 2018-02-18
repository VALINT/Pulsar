 /* "PULSAR library"
 *
 * Author: VAL
 * Created: 12.02.2018 13:04:21
 * Version: 1.2
 *
 * V 1.0
 * This is a simplified library needed for managing the display 
 * with a HD44780 controller via one wire. Works only in 4-bit mode. 
 * One-wire control is implemented using the shift register SN74HC595. 
 * In theory, you can using SN74HC164, but I didn't check it.
 *
 * V 1.1
 * I modified the library to work with SN74HC164.
 *
 * V 1.2
 * Added support of Cyrillic alphabet 
 */ 


#ifndef PULSARHD44780_H_
#define PULSARHD44780_H_

#define F_CPU 8000000Ul


#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

//---------------------------------------------------------------------------------------
// Uncomment this definition if you need to use shift register SN74HC164 or his analog.
//#define USE_SN74HC164

//---------------------------------------------------------------------------------------
// You can variate this delay for faster or most stability work
#define STROBE_DELAY_MS 50

//---------------------------------------------------------------------------------------
// Definitions for using in functions
#define COMMAND			0x00
#define DATA			0x01

#define CURSOR_OFF		0x00
#define CURSOR_ON		0x02
#define CURSOR_BLINK	0x03

#define DISPLAY_OFF		0x00
#define DISPLAY_ON		0x04

#define SCROLL_LEFT		0x00
#define SCROLL_RIGHT	0x04

//---------------------------------------------------------------------------------------
// In this case you can select the PORT and PIN to be used for data transfer.
#define DATAPIN			0
#define DATAPORT		PORTD
#define DATADDR			DDRD
// In this case you can select the PORT and PIN to be used for strobe.
// Only for shift register SN74HC164 or his analog.
#define EPIN			4
#define EPORT			PORTD
#define EDDR			DDRD

//----------------------------------------------------------------------------------------
//	Sets cursor on desired position.
		void LCD_GotoXY(uint8_t line, uint8_t coloum);
	
//	Scrolls the visible part in the desired direction and on the required number of characters
//	void LCD_Scroll(1, SCROLL_LEFT);
		void LCD_Scroll(uint8_t number, uint8_t dir);
	
//	Sent text data in the LCD
		void LCD_SentLine(char line[],uint8_t lon);
	
//	Sent data byte or command in the LCD
//	LCD_SentByte(data_byte,DATA)
//	LCD_SentByte(aommand_number,COMMAND)
		void LCD_SentByte(uint8_t byte,bool rs);
	
//	Sent data byte in the LCD (analog LCD_SentByte(data_byte,DATA))
		void LCD_SentChar(uint8_t data);
		
//	Sets desired cursor mode
//	LCD_Cursor(CURSOR_BLINK);
		void LCD_Cursor(uint8_t cursor);
		
//	You can turn on LCD or turn off his
//	LCD_State(DISPLAY_ON);
		void LCD_State(uint8_t state);
		
// This function initializations LCD		
		void LCD_Initial(void);
		
// This function clears LCD		
		void LCD_Clear(void);

		

#endif /* PULSARHD44780_H_ */