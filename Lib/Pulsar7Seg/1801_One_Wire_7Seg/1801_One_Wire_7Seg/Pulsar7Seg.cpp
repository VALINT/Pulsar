/*
 * _Pulsar7SegD.c
 *
 * Created: 05.01.2018 20:29:51
 *  Author: VAL
 */ 

#include "Pulsar7Seg.h"


// 

#ifndef DISPLAY_TIME	// Setting default value of Display time
#define DISPLAY_TIME 1
#endif

#define SHOW_SYMBOL (_delay_us(DISPLAY_TIME))

#ifndef DECIMAL_POINT
#define FREE_POINT
#endif

#if DECIMAL_POINT == 0
#define FREE_POINT
#endif

#ifndef SYMBOLS			// Setting Default number of symbols in display
#define SYMBOLS 4
#endif

#ifdef USE_MCU_CONTROL		// Defined macros for mode 1
	#if SYMBOLS == 4		// Define service macros
		#define SHOW_ONE	DIG1PORT |= (1 << DIG1PIN)
		#define	HIDE_ONE	DIG1PORT &=~(1 << DIG1PIN)
		#define SHOW_TWO	DIG2PORT |= (1 << DIG2PIN)
		#define HIDE_TWO	DIG2PORT &=~(1 << DIG2PIN)
		#define SHOW_THREE	DIG3PORT |= (1 << DIG3PIN)
		#define HIDE_THREE	DIG3PORT &=~(1 << DIG3PIN) 
		#define SHOW_FOUR	DIG4PORT |= (1 << DIG4PIN)
		#define HIDE_FOUR	DIG4PORT &=~(1 << DIG4PIN) 
	#elif SYMBOLS == 3
		#define SHOW_ONE	DIG1PORT |= (1 << DIG1PIN)
		#define	HIDE_ONE	DIG1PORT &=~(1 << DIG1PIN)
		#define SHOW_TWO	DIG2PORT |= (1 << DIG2PIN)
		#define HIDE_TWO	DIG2PORT &=~(1 << DIG2PIN)
		#define SHOW_THREE	DIG3PORT |= (1 << DIG3PIN)
		#define HIDE_THREE	DIG3PORT &=~(1 << DIG3PIN)
	#elif SYMBOLS == 2
		#define SHOW_ONE	DIG1PORT |= (1 << DIG1PIN)
		#define	HIDE_ONE	DIG1PORT &=~(1 << DIG1PIN)
		#define SHOW_TWO	DIG2PORT |= (1 << DIG2PIN)
		#define HIDE_TWO	DIG2PORT &=~(1 << DIG2PIN)
	#elif SYMBOLS == 1
		#define SHOW_ONE	DIG1PORT |= (1 << DIG1PIN)
		#define	HIDE_ONE	DIG1PORT &=~(1 << DIG1PIN)
	#else
		#error "Select the number of showing sights in display menu in header this library"
	#endif
#endif


#ifdef USE_SERIAL_CONTROL	// Defined macros for mode 2
	#if SYMBOLS == 4		// Define service macros
	#define SHOW_ONE	asm("nop")
	#define	HIDE_ONE	SentByte(0b00010000)
	#define SHOW_TWO	asm("nop")
	#define HIDE_TWO	SentByte(0b10000000)
	#define SHOW_THREE	asm("nop")
	#define HIDE_THREE	SentByte(0b01000000)
	#define SHOW_FOUR	asm("nop")
	#define HIDE_FOUR	SentByte(0b00100000)
	#elif SYMBOLS == 3
	#define SHOW_ONE	asm("nop")
	#define	HIDE_ONE	SentByte(0b00100000)
	#define SHOW_TWO	asm("nop")
	#define HIDE_TWO	SentByte(0b10000000)
	#define SHOW_THREE	asm("nop")
	#define HIDE_THREE	SentByte(0b01000000)
	#elif SYMBOLS == 2
	#define SHOW_ONE	asm("nop")
	#define	HIDE_ONE	SentByte(0b01000000)
	#define SHOW_TWO	asm("nop")
	#define HIDE_TWO	SentByte(0b10000000)
	#elif SYMBOLS == 1
	#define SHOW_ONE	asm("nop")
	#define	HIDE_ONE	SentByte(0b10000000)
	#else
	#error "Select the number of showing sights in display menu in header this library"
	#endif
#endif


#ifdef USE_STATIC_MODE		// Defined macros for mode 3
	#define SHOW_SYMBOL asm("nop")
	#if SYMBOLS == 4		// Define service macros
		#define SHOW_ONE	asm("nop")
		#define	HIDE_ONE	asm("nop")
		#define SHOW_TWO	asm("nop")
		#define HIDE_TWO	asm("nop")
		#define SHOW_THREE	asm("nop")
		#define HIDE_THREE	asm("nop")
		#define SHOW_FOUR	asm("nop")
		#define HIDE_FOUR	asm("nop")
	#elif SYMBOLS == 3
		#define SHOW_ONE	asm("nop")
		#define	HIDE_ONE	asm("nop")
		#define SHOW_TWO	asm("nop")
		#define HIDE_TWO	asm("nop")
		#define SHOW_THREE	asm("nop")
		#define HIDE_THREE	asm("nop")
	#elif SYMBOLS == 2
		#define SHOW_ONE	asm("nop")
		#define	HIDE_ONE	asm("nop")
		#define SHOW_TWO	asm("nop")
		#define HIDE_TWO	asm("nop")
	#elif SYMBOLS == 1
		#define SHOW_ONE	asm("nop")
		#define	HIDE_ONE	asm("nop")
	#else
		#error "Select the number of showing sights in display menu in header this library"
	#endif
#endif
// Systems functions, don't touch it!
void SentBit(bool bit); // This function write one bit in shift register

// So stange construction but it works!!
#if   SYMBOLS == 4		// This functions are filling display respectively settings
void FillDisplayF(uint8_t FirstDig, uint8_t SecondDig, uint8_t ThirdDig, uint8_t FourthDig, uint8_t DigPoint);	
void FillDisplayI(uint8_t FirstDig, uint8_t SecondDig, uint8_t ThirdDig, uint8_t FourthDig);	
#elif SYMBOLS == 3
void FillDisplayF(uint8_t FirstDig, uint8_t SecondDig, uint8_t ThirdDig, uint8_t DigPoint);
void FillDisplayI(uint8_t FirstDig, uint8_t SecondDig, uint8_t ThirdDig);											
#elif SYMBOLS == 2
void FillDisplayF(uint8_t FirstDig, uint8_t SecondDig, uint8_t DigPoint);
void FillDisplayI(uint8_t FirstDig, uint8_t SecondDig);				
#elif SYMBOLS == 1
void FillDisplayF(uint8_t FirstDig);
#endif


// 
const uint8_t SevenSegSymbols [] PROGMEM =	
{
	0b11111100,		//0
	0b01100000,		//1
	0b11011010,		//2
	0b11110010,		//3
	0b01100110,		//4
	0b10110110,		//5
	0b10111110,		//6
	0b11100000,		//7
	0b11111110,		//8
	0b11110110		//9
};

void DisplayInit(void)						// This function initialized MCU PINs
{
	DATADDR |= (1 << DATAPIN);
	DATAPORT|= (1 << DATAPIN);
	#ifdef USE_MCU_CONTROL
		#if SYMBOLS == 4
		DIG1DDR |= (1 << DIG1PIN);
		DIG2DDR |= (1 << DIG2PIN);
		DIG3DDR |= (1 << DIG3PIN);
		DIG4DDR |= (1 << DIG4PIN);
		#elif SYMBOLS == 3
		DIG1DDR |= (1 << DIG1PIN);
		DIG2DDR |= (1 << DIG2PIN);
		DIG3DDR |= (1 << DIG3PIN);
		#elif SYMBOLS == 2
		DIG1DDR |= (1 << DIG1PIN);
		DIG2DDR |= (1 << DIG2PIN);
		#elif SYMBOLS == 1
		DIG1DDR |= (1 << DIG1PIN);
		#endif
	#endif
}

void SentBit(bool bit)						// This function sent bit in shift register short pulse if TRUE, long pulse is FALSE.
{
	if(bit)																	// Short pulse 1 (True)
	{	DATAPORT &=~ (1 << DATAPIN);
		asm("nop");
		DATAPORT |= (1 << DATAPIN);}
	else																	// Long pulse 0 (False)
	{	DATAPORT &=~ (1 << DATAPIN);
		_delay_us(3);
		DATAPORT |= (1 << DATAPIN);}
	_delay_us(3);															// Wait until capacitor recharged
}

void SentByte(uint8_t number, bool point)	// This function sent byte in shift register
{
	number = pgm_read_byte(&(SevenSegSymbols[number]));
	if (point)	{SentBit(1);number >>= 1;}
	else		{SentBit(0);number >>= 1;}
	#ifdef CA
		number =~ number;
	#endif
	for(uint8_t bit = 0; bit < 7; bit++)
	{
		if((number & 0b00000001) == 1)SentBit(1);
		else SentBit(0);
		number >>= 1;
	}
	#ifdef USE_MCU_CONTROL
	_delay_us(20);
	#endif
}

void SentByte(uint8_t number)	// This function sent byte in shift register
{
	for(uint8_t bit = 0; bit < 8; bit++)
	{
		if((number & 0b00000001) == 1)SentBit(1);
		else SentBit(0);
		number >>= 1;
	}
}

void PrintInteger(uint16_t value)			// This function share whole number on digits respectively settings
{
	#if SYMBOLS == 4														// This part will be compiled if need to show four symbols
		uint8_t		FirstDig = (value%10000)/1000,							// Value when you want show in display divided on four digits
					SecondDig = (value%1000)/100,							// On display will be show first four digits it's units tens hundreds and thousandths
					ThirdDig = (value%100)/10,
					FourthDig = value%10;
		FillDisplayI(FirstDig,SecondDig,ThirdDig,FourthDig);
	#elif SYMBOLS == 3														// This part will be compiled if need to show three symbols
		uint8_t		FirstDig = (value%1000)/100,							// Value when you want show in display divided on three digits
					SecondDig = (value%100)/10,								// On display will be show first four digits it's units tens and hundreds
					ThirdDig = (value%100)%10;
		FillDisplayI(FirstDig,SecondDig,ThirdDig);
	#elif SYMBOLS == 2
		uint8_t		FirstDig = (value%100)/10,
					SecondDig = value%10;
		FillDisplayI(FirstDig,SecondDig);
	#elif SYMBOLS == 1
		uint8_t		FirstDig = value%10;
		FillDisplayI(FirstDig);
	#endif
}

void PrintFloat(float value)				// This function share floating-point number on digits respectively settings			
{
	#ifdef FREE_POINT
		#if SYMBOLS == 4
			uint16_t Decimal_Part, Fractional_Part; 
			uint8_t First_Digit, Second_Digit, Third_digit, Fourth_Digit, point;
			Decimal_Part = value;
			if(Decimal_Part < 10) {Decimal_Part = (value*1000); point = 1;}
			else if(Decimal_Part < 100) {Decimal_Part = (value*100); point = 2;}
			else if(Decimal_Part < 1000) {Decimal_Part = (value*10); point = 3;}
			else if(Decimal_Part < 10000) {Decimal_Part = (value*1); point = 4;}
			
			First_Digit = (Decimal_Part%10000)/1000,							
			Second_Digit = (Decimal_Part%1000)/100,							
			Third_digit = (Decimal_Part%100)/10,
			Fourth_Digit = Decimal_Part%10;
			FillDisplayF(First_Digit, Second_Digit, Third_digit, Fourth_Digit, point);
			
		#elif SYMBOLS == 3
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, point;
			Decimal_Part = value;
			if(Decimal_Part < 10) {Decimal_Part = (value*100); point = 1;}
			else if(Decimal_Part < 100) {Decimal_Part = (value*10); point = 2;}
			else if(Decimal_Part < 1000) {Decimal_Part = (value); point = 3;}
			
			First_Digit = (Decimal_Part%1000)/100,							
			Second_Digit = (Decimal_Part%100)/10,							
			Third_digit = Decimal_Part%10,
			FillDisplayF(First_Digit, Second_Digit, Third_digit, point);
			
		#elif SYMBOLS == 2
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, point;
			Decimal_Part = value;
			if(Decimal_Part < 10) {Decimal_Part = (value*10); point = 1;}
			else if(Decimal_Part < 100) {Decimal_Part = (value); point = 2;}
		

			First_Digit = (Decimal_Part%100)/10,
			Second_Digit = Decimal_Part%10,
			
			FillDisplayF(First_Digit, Second_Digit, point);
		#endif
	#endif
	
	#if DECIMAL_POINT == 4
		#if SYMBOLS == 4
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, Fourth_Digit;
			Decimal_Part = (value);
			
			First_Digit = (Decimal_Part%10000)/1000,							
			Second_Digit = (Decimal_Part%1000)/100,							
			Third_digit = (Decimal_Part%100)/10,
			Fourth_Digit = Decimal_Part%10;
			FillDisplayF(First_Digit, Second_Digit, Third_digit, Fourth_Digit, 4);
		#else
			#error "Not Enough symbols"
		#endif
		
	#elif DECIMAL_POINT == 3
	
		#if SYMBOLS == 4
			
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, Fourth_Digit;
			Decimal_Part = value*10;
			
			First_Digit = (Decimal_Part%10000)/1000,							
			Second_Digit = (Decimal_Part%1000)/100,							
			Third_digit = (Decimal_Part%100)/10,
			Fourth_Digit = Decimal_Part%10;
			FillDisplayF(First_Digit, Second_Digit, Third_digit, Fourth_Digit, 3);
			
		#elif SYMBOLS == 3
			
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, point;
			Decimal_Part = value;

			First_Digit = (Decimal_Part%1000)/100,
			Second_Digit = (Decimal_Part%100)/10,
			Third_digit = Decimal_Part%10,
			FillDisplayF(First_Digit, Second_Digit, Third_digit, 3);
			
		#else
		#error "Not Enough symbols"
		#endif
		
	#elif DECIMAL_POINT == 2
	
		#if SYMBOLS == 4
		
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, Fourth_Digit;
			Decimal_Part = value*100;
		
			First_Digit = (Decimal_Part%10000)/1000,
			Second_Digit = (Decimal_Part%1000)/100,
			Third_digit = (Decimal_Part%100)/10,
			Fourth_Digit = Decimal_Part%10;
			
			FillDisplayF(First_Digit, Second_Digit, Third_digit, Fourth_Digit, 2);
		
		#elif SYMBOLS == 3
		
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, point;
			Decimal_Part = value*10;

			First_Digit = (Decimal_Part%1000)/100,
			Second_Digit = (Decimal_Part%100)/10,
			Third_digit = Decimal_Part%10,
			
			FillDisplayF(First_Digit, Second_Digit, Third_digit, 2);
		
		#elif SYMBOLS == 2
		
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, point;
			Decimal_Part = value;

			First_Digit = (Decimal_Part%100)/10,
			Second_Digit = Decimal_Part%10,
	
			FillDisplayF(First_Digit, Second_Digit, 2);
		
		#else
		#error "Not Enough symbols"
		#endif
		
	#elif DECIMAL_POINT == 1
	
		#if SYMBOLS == 4
		
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, Fourth_Digit;
			Decimal_Part = value*1000;
		
			First_Digit = (Decimal_Part%10000)/1000,
			Second_Digit = (Decimal_Part%1000)/100,
			Third_digit = (Decimal_Part%100)/10,
			Fourth_Digit = Decimal_Part%10;
			FillDisplayF(First_Digit, Second_Digit, Third_digit, Fourth_Digit, 1);
		
		#elif SYMBOLS == 3
		
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, point;
			Decimal_Part = value*100;

			First_Digit = (Decimal_Part%1000)/100,
			Second_Digit = (Decimal_Part%100)/10,
			Third_digit = Decimal_Part%10,
			FillDisplayF(First_Digit, Second_Digit, Third_digit, 1);
		
		#elif SYMBOLS == 2
		
			uint16_t Decimal_Part, Fractional_Part;
			uint8_t First_Digit, Second_Digit, Third_digit, point;
			Decimal_Part = value*10;

			First_Digit = (Decimal_Part%100)/10,
			Second_Digit = Decimal_Part%10,
			
			FillDisplayF(First_Digit, Second_Digit, 1);
		
		#else
		#error "Not Enough symbols"
		#endif
		
	#endif
}
// So strange construction but it works!!
#if     SYMBOLS == 4						// This functions filling display respectively settings
void FillDisplayF(uint8_t FirstDig, uint8_t SecondDig, uint8_t ThirdDig, uint8_t FourthDig, uint8_t DigPoint)
{
	HIDE_ONE;
	if(DigPoint == 4)SentByte(FourthDig,1);
	else SentByte(FourthDig,0);
	SHOW_FOUR;
	SHOW_SYMBOL;
	
	HIDE_FOUR;
	if(DigPoint == 3)SentByte(ThirdDig,1);
	else SentByte(ThirdDig,0);
	SHOW_THREE;
	SHOW_SYMBOL;
	
	HIDE_THREE;
	if(DigPoint == 2)SentByte(SecondDig,1);
	else SentByte(SecondDig,0);
	SHOW_TWO;
	SHOW_SYMBOL;
	
	HIDE_TWO;
	if(DigPoint == 1)SentByte(FirstDig,1);
	else SentByte(FirstDig,0);
	SHOW_ONE;
	SHOW_SYMBOL;		
}		
void FillDisplayI(uint8_t FirstDig, uint8_t SecondDig, uint8_t ThirdDig, uint8_t FourthDig)
{
	#ifdef HIDE_INSIGNIFICANT_ZEROS										// This part will be compiled if need to hide insignificant zeros
		
		HIDE_ONE;
		SentByte(FourthDig,0);
		SHOW_FOUR;
		SHOW_SYMBOL;
			
		if(FirstDig == 0 && SecondDig == 0 && ThirdDig == 0)
		{	HIDE_FOUR;
			SentByte(0);}
		else
		{	HIDE_FOUR;
			SentByte(ThirdDig,0);
			SHOW_THREE;
			SHOW_SYMBOL;}
		
		if(FirstDig == 0 && SecondDig ==0)
		{	HIDE_THREE;
			SentByte(0);}
		else
		{	HIDE_THREE;
			SentByte(SecondDig,0);
			SHOW_TWO;
			SHOW_SYMBOL;}
			
		if(FirstDig == 0)
		{	HIDE_TWO;
			SentByte(0);}
		else
		{	HIDE_TWO;
			SentByte(FirstDig,0);
			SHOW_ONE;
			SHOW_SYMBOL;}

	#else
		HIDE_ONE;
		SentByte(FourthDig,0);
		SHOW_FOUR;
		SHOW_SYMBOL;	
	
		HIDE_FOUR;
		SentByte(ThirdDig,0);
		SHOW_THREE;
		SHOW_SYMBOL;
	
		HIDE_THREE;
		SentByte(SecondDig,0);
		SHOW_TWO;
		SHOW_SYMBOL;

																			// This part will be compiled if need to show insignificant zeros
		HIDE_TWO;
		SentByte(FirstDig,0);
		SHOW_ONE;
		SHOW_SYMBOL;

	#endif
}
#elif   SYMBOLS == 3
void FillDisplayF(uint8_t FirstDig, uint8_t SecondDig, uint8_t ThirdDig, uint8_t DigPoint)
{
	
	HIDE_ONE;
	if(DigPoint == 3)SentByte(ThirdDig,1);
	else SentByte(ThirdDig,0);
	SHOW_THREE;
	SHOW_SYMBOL;
	
	HIDE_THREE;
	if(DigPoint == 2)SentByte(SecondDig,1);
	else SentByte(SecondDig,0);
	SHOW_TWO;
	SHOW_SYMBOL;
	
	HIDE_TWO;
	if(DigPoint == 1)SentByte(FirstDig,1);
	else SentByte(FirstDig,0);
	SHOW_ONE;
	SHOW_SYMBOL;

}	
void FillDisplayI(uint8_t FirstDig, uint8_t SecondDig, uint8_t ThirdDig)
{
	#ifdef HIDE_INSIGNIFICANT_ZEROS
			
		HIDE_ONE;
		SentByte(ThirdDig,0);
		SHOW_THREE;
		SHOW_SYMBOL;
			
		if(FirstDig == 0 && SecondDig ==0)
		{	HIDE_THREE;
			SentByte(0);}
		else
		{	HIDE_THREE;
			SentByte(SecondDig,0);
			SHOW_TWO;
			SHOW_SYMBOL;}
				
		if(FirstDig == 0)
		{	HIDE_TWO;
			SentByte(0);}
		else
		{	HIDE_TWO;
			SentByte(FirstDig,0);
			SHOW_ONE;
			SHOW_SYMBOL;}

	#else
		
		HIDE_ONE;
		SentByte(ThirdDig,0);
		SHOW_THREE;
		SHOW_SYMBOL;
		
		HIDE_THREE;
		SentByte(SecondDig,0);
		SHOW_TWO;
		SHOW_SYMBOL;
				
		HIDE_TWO;
		SentByte(FirstDig,0);
		SHOW_ONE;
		SHOW_SYMBOL;

	#endif
}
#elif   SYMBOLS == 2
void FillDisplayF(uint8_t FirstDig, uint8_t SecondDig, uint8_t DigPoint)
{
	
	HIDE_ONE;
	if(DigPoint == 2)SentByte(SecondDig,1);
	else SentByte(SecondDig,0);
	SHOW_TWO;
	SHOW_SYMBOL;
		
	HIDE_TWO;
	if(DigPoint == 1)SentByte(FirstDig,1);
	else SentByte(FirstDig,0);
	SHOW_ONE;
	SHOW_SYMBOL;

}
void FillDisplayI(uint8_t FirstDig, uint8_t SecondDig)
{
	#ifdef HIDE_INSIGNIFICANT_ZEROS
		
		HIDE_ONE;
		SentByte(SecondDig);
		SHOW_TWO;
		SHOW_SYMBOL;
			
		if(FirstDig == 0)
		{	HIDE_TWO;
			SentByte(0);}
		else
		{	HIDE_TWO;
			SentByte(FirstDig);
			SHOW_ONE;
			SHOW_SYMBOL;}

	#else
			
		DIG1PORT &=~ (1 << DIG1PIN);
		SentByte(SecondDig,0);
		DIG1PORT |= (1 << DIG2PIN);
		SHOW_SYMBOL;	
			
		DIG1PORT &=~ (1 << DIG2PIN);
		SentByte(FirstDig,0);
		DIG1PORT |= (1 << DIG1PIN);
		SHOW_SYMBOL;

	#endif
	
}
#elif   SYMBOLS == 1
void FillDisplayI(uint8_t FirstDig)
{		
		DIG1PORT &=~ (1 << DIG1PIN);
		SentByte(FirstDig);
		DIG1PORT|= (1 << DIG1PIN);
}
#endif