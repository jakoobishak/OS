
/*
 * lcd.h
 *
 *  Created on: Apr 27, 2021
 *      Author: axeand
 *
 *  Description:
 *  	LCD driver for 16x2 LCD based on HD44780U and using PCF8574 I2C I/O-expander
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_
#include "main.h"

/*
 *  PCF8574 I/O-expander
 *  P0 - RS
 *  P1 - RW
 *  P2 - E
 *  P3 - BT
 *  P4 - D4
 *  P5 - D5
 *  P6 - D6
 *  P7 - D7
 */

typedef struct TextLCDType
{
	I2C_HandleTypeDef *hi2c;
	uint8_t DevAddress;
	TIM_HandleTypeDef* htim;
	uint8_t data;

} TextLCDType;

void TextLCD_Init(TextLCDType *lcd, I2C_HandleTypeDef *hi2c, uint8_t DevAddress, TIM_HandleTypeDef* htim);

void TextLCD_Hello(TextLCDType *lcd);
void TextLCD_Home(TextLCDType *lcd);
void TextLCD_Clear(TextLCDType *lcd);
void TextLCD_Position(TextLCDType *lcd, int x, int y);
void TextLCD_Putchar(TextLCDType *lcd, uint8_t ascii_char);
void TextLCD_Puts(TextLCDType *lcd, char *string);
//void TextLCD_Printf(TextLCDType *lcd, char *message, …);
void TextLCD_CustomChar(TextLCDType *lcd, uint8_t adr, uint8_t data);

void TextLCD_udelay(TextLCDType *lcd, uint32_t delay_usec);

#endif /* INC_LCD_H_ */

