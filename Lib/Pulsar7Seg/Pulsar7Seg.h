 /* "PULSAR library"
 *
 * Author: VAL
 * Created: 05.01.2018 20:30:13
 * Version: 1.1
 *
 * The main task of this library is to manage seven-segment indicators by one wire.
 * In the project files, you can find the schematic diagram of connections of the displays to the microcontroller.
 * The given library has three modes of operation.
 * 1 - mode where the symbol is loaded into the shift register and the bits are controlled by the microcontroller.
 * 2 - the mode in which both the symbol and the digit of the digit are written to the shift register.
 * 3 - the mode of static indication in which all the symbols are immediately written into shift registers.
 * You can configure the operating modes in the menu below.
 */
 
#ifndef PULSAR7SEG_H_
#define PULSAR7SEG_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//-------------------------------------------------------------------------
// Showing menu
//-------------------------------------------------------------------------
// In this case, you can select the number of displayed characters
// Can be 1, 2, 3 or 4.
// Default value is 4

#define SYMBOLS 4

//-------------------------------------------------------------------------
// In this case you can lock a decimal point
//
//#define DECIMAL_POINT 1 or 2 or 3 or 4 or 0
//#define FREE_POINT
// Default is FREE_POINT

#define FREE_POINT

//-------------------------------------------------------------------------
// In this case you can show or hide insignificant zeros
// If nothing choosed, insignificant zeros will be shows
// In 
// Default is show 
//#define SHOW_INSIGNIFICANT_ZEROS
//#define HIDE_INSIGNIFICANT_ZEROS

#define HIDE_INSIGNIFICANT_ZEROS

//-------------------------------------------------------------------------
// In this case you can select type of LED display
// CC (Common cathode) or CA (Common anode)
// Default is CC

#define CC

//-------------------------------------------------------------------------
// In this case you can select method of organization dynamic indication
//#define USE_MCU_CONTROL				- mode 1 Dynamic indication, digit control with MCU
//#define USE_SERIAL_CONTROL			- mode 2 Dynamic indication, digit control with additional shift register, 
//#define USE_STATIC_MODE 				- mode 3 Static indication, one shift register for one digit

#define USE_MCU_CONTROL

//-------------------------------------------------------------------------
// In this part you can select the display time of one symbol in ms
// If you did not select this value, will be used default value 1 ms 

#define DISPLAY_TIME 1000

//-------------------------------------------------------------------------
// In this case, you can select the PORT PIN to be used for data transfer.
#define DATAPIN		0
#define DATAPORT	PORTD
#define DATADDR		DDRD
// In this case, you can select the PORT PIN to be used for showing first digit. 
// Uncomment if you will be use this digit.
#define DIG1PIN		1
#define DIG1PORT	PORTD
#define DIG1DDR		DDRD
// In this case, you can select the PORT PIN to be used for showing second digit. 
// Uncomment if you will be use this digit.
#define DIG2PIN		2
#define DIG2PORT	PORTD
#define DIG2DDR		DDRD
// In this case, you can select the PORT PIN to be used for showing third digit. 
// Uncomment if you will be use this digit.
#define DIG3PIN		3
#define DIG3PORT	PORTD
#define DIG3DDR		DDRD
// In this case, you can select the PORT PIN to be used for showing fourth digit. 
// Uncomment if you will be use this digit.
#define DIG4PIN		4
#define DIG4PORT	PORTD
#define DIG4DDR		DDRD

// User functions 
void DisplayInit(void);
void SentByte(uint8_t number, bool point);// This function sent byte in shift register short pulse is TRUE, long pulse is FALSE.
void SentByte(uint8_t number);
void PrintInteger(uint16_t value);
void PrintFloat(float value);


#endif /* PULSAR7SEGD_H_ */