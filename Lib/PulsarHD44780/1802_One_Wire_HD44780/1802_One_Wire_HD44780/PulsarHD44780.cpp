/*
 * PulsarHD44780.cpp
 *
 * Created: 12.02.2018 13:04:21
 *  Author: VAL
 */ 

#include "PulsarHD44780.h"

void SentBit(bool bit);
void SentOutByte(uint8_t number);
void SentFourBits(uint8_t data);
char ConvertToCyrillic(char str);


uint8_t out = 0;								// This byte is generated for sending in the shift register


void SentBit(bool bit)							// This function sent bit in shift register short pulse if TRUE, long pulse is FALSE.
{
	if(bit)										// Short pulse 1 (True)
	{	DATAPORT &=~ (1 << DATAPIN);
		asm("nop");
	DATAPORT |= (1 << DATAPIN);}
	else										// Long pulse 0 (False)
	{	DATAPORT &=~ (1 << DATAPIN);
		_delay_us(5);
	DATAPORT |= (1 << DATAPIN);}
	_delay_us(5);								// Wait until capacitor recharged
}

void SentOutByte(uint8_t number)				// This function sent byte in shift register
{
	for(uint8_t bit = 0; bit < 8; bit++)
	{
		if((number & 0b00000001) == 1)SentBit(1);
		else SentBit(0);
		number >>= 1;
	}
	_delay_us(150);
}

void SentFourBits(uint8_t data)					// This function sent four bits, signal RW and signal E in the shift register.
{
	out &=~ (0b00001111);
	out |= (0b00010000);						//Set E in 1 - transmission beginning.
	SentOutByte(out);
	#ifdef USE_SN74HC164						//Use only in SN74HC164 mode. Additional 'E' (Strobe) signal.
		EPORT |= (1 << EPIN);
	#endif
	out =  out| data;
	SentOutByte(out);
	out &= (0b11101111);						//Set E in 0 - transmission finishing.
	SentOutByte(out);
	#ifdef USE_SN74HC164
		_delay_ms(1);
		EPORT &=~ (1 << EPIN);					//Use only in SN74HC164 mode. Additional 'E' (Strobe) signal.
	#endif
	_delay_us(STROBE_DELAY_MS);					// You can variate this delay for faster or most stability work
}

void LCD_SentByte(uint8_t byte,bool rs)			// This function sent byte in the LCD
{
	uint8_t highbyte = 0;
	highbyte = byte>>4;							// This part separates byte on two senior and younger parts, needed for 4-bits mode
	if(rs)	out |= (0b00100000);				// DATA / COMMAND
	else	out &=~ (0b00100000);				// rs = 0 - COMMAND rs = 1 - DATA
	SentFourBits(highbyte);						// transmission of the senior and younger part of the byte, consistently
	SentFourBits(byte);
}

void LCD_SentChar(uint8_t data)					// This function sent data byte in the LCD
{
	uint8_t highbyte = 0;
	highbyte = data>>4;
	out |= (0b00100000);
	SentFourBits(highbyte);
	SentFourBits(data);
}

void LCD_Initial(void)							// This function initializations LCD
{
	DATADDR |=  (1 << DATAPIN);
	
	#ifdef USE_SN74HC164
	EDDR	|= (1 << EPIN);
	#endif
	
	_delay_ms(20);									//----------------------------------
	SentFourBits(0b00000011);						//  
	_delay_ms(5);									//	
	SentFourBits(0b00000011);						//
	_delay_us(100);									//
	SentFourBits(0b00000011);						//
	_delay_ms(1);									//
	SentFourBits(0b00000010);						//
	_delay_ms(1);									//
	LCD_SentByte(0b00101100, COMMAND);				//
	_delay_ms(1);									//
	LCD_SentByte(0b00001100, COMMAND);				//
	_delay_ms(1);									//
	LCD_SentByte(0b00000001, COMMAND);				//
	_delay_ms(1);									//
	LCD_SentByte(0b00000110, COMMAND);				//
	_delay_ms(1);									//------------------------
}

void LCD_Clear(void)								// This function clears LCD		
{
	_delay_ms(1);
	LCD_SentByte(0b00000001, COMMAND);						
	_delay_ms(2);
}

void LCD_GotoXY(uint8_t line, uint8_t coloum)		// Sets cursor on desired position.
{
	uint8_t adres;
	adres = 0x40*line+coloum;
	adres |= 0x80;
	LCD_SentByte(adres, COMMAND);	
}

void LCD_State(uint8_t state)						//	You can turn on LCD or turn off his
{
	LCD_SentByte((0b00001000|state), COMMAND);
}

void LCD_Cursor(uint8_t cursor)						//	Sets desired cursor mode
{
	LCD_SentByte((0b00001100|cursor), COMMAND);
}

void LCD_Scroll(uint8_t number, uint8_t dir)		//	Scrolls the visible part in the desired direction and on the required number of characters
{
	for(uint8_t i = 0; i < number; i++)
	{
		LCD_SentByte((0b00011000|dir), COMMAND);
	}
}

void LCD_SentLine(char line[],uint8_t lon)			//	Sent text data in the LCD
{
	for(uint8_t i = 0;i <= lon; i++)
	{
		LCD_SentChar(ConvertToCyrillic(line[i]));
	}
}

char ConvertToCyrillic(char str)
{
		switch (str)
	{
		case 'À': return 0x41; break;      // À
		case 'Á': return 0xA0; break;      // Á
		case 'Â': return 0x42; break;      // Â
		case 'Ã': return 0xA1; break;      // Ã
		case 'Ä': return 0xE0; break;      // Ä
		case 'Å': return 0x45; break;      // Å
		case '¨': return 0xA2; break;      // ¨
		case 'Æ': return 0xA3; break;      // Æ
		case 'Ç': return 0x33; break;      // Ç
		case 'È': return 0xA5; break;      // È
		case 'É': return 0xA6; break;      // É
		case 'Ê': return 0x4B; break;      // Ê
		case 'Ë': return 0xA7; break;      // Ë
		case 'Ì': return 0x4D; break;      // Ì
		case 'Í': return 0x48; break;      // Í
		case 'Î': return 0x4F; break;      // Î
		case 'Ï': return 0xA8; break;      // Ï
		case 'Ð': return 0x50; break;      // Ð
		case 'Ñ': return 0x43; break;      // Ñ
		case 'Ò': return 0x54; break;      // Ò
		case 'Ó': return 0xA9; break;      // Ó
		case 'Ô': return 0xAA; break;      // Ô
		case 'Õ': return 0x58; break;      // Õ
		case 'Ö': return 0xE1; break;      // Ö
		case '×': return 0xAB; break;      // ×
		case 'Ø': return 0xAC; break;      // Ø
		case 'Ù': return 0xE2; break;      // Ù
		case 'Ú': return 0xAD; break;      // Ú
		case 'Û': return 0xAE; break;      // Û
		case 'Ü': return 0x62; break;      // Ü
		case 'Ý': return 0xAF; break;      // Ý
		case 'Þ': return 0xB0; break;      // Þ
		case 'ß': return 0xB1; break;      // ß
		case 'à': return 0x61; break;      // à
		case 'á': return 0xB2; break;      // á
		case 'â': return 0xB3; break;      // â
		case 'ã': return 0xB4; break;      // ã
		case 'ä': return 0xE3; break;      // ä
		case 'å': return 0x65; break;      // å
		case '¸': return 0xB5; break;      // ¸
		case 'æ': return 0xB6; break;      // æ
		case 'ç': return 0xB7; break;      // ç
		case 'è': return 0xB8; break;      // è
		case 'é': return 0xB9; break;      // é
		case 'ê': return 0xBA; break;      // ê
		case 'ë': return 0xBB; break;      // ë
		case 'ì': return 0xBC; break;      // ì
		case 'í': return 0xBD; break;      // í
		case 'î': return 0x6F; break;      // î
		case 'ï': return 0xBE; break;      // ï
		case 'ð': return 0x70; break;      // ð
		case 'ñ': return 0x63; break;      // ñ
		case 'ò': return 0xBF; break;      // ò
		case 'ó': return 0x79; break;      // ó
		case 'ô': return 0xE4; break;      // ô
		case 'õ': return 0x78; break;      // õ
		case 'ö': return 0xE5; break;      // ö
		case '÷': return 0xC0; break;      // ÷
		case 'ø': return 0xC1; break;      // ø
		case 'ù': return 0xE6; break;      // ù
		case 'ú': return 0xC2; break;      // ú
		case 'û': return 0xC3; break;      // û
		case 'ü': return 0xC4; break;      // ü
		case 'ý': return 0xC5; break;      // ý
		case 'þ': return 0xC6; break;      // þ
		case 'ÿ': return 0xC7; break;      // ÿ
		default : return str;
	}
}