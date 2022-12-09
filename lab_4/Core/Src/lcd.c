
/*
 * lcd.c
 *
 *  Created on: Apr 27, 2021
 *      Author: axeand
 *              alexander goransson
 */
#include "lcd.h"
// D7 D6 D5 D4 BT E  RW RS
// 4xdata, backlight (not working), enable/strobe, RW, RS

#define STROBE_DELAY_USEC 40

// Works because TIM1 runs at 1 MHz
void TextLCD_udelay(TextLCDType *lcd, uint32_t delay_usec)
{
	volatile uint32_t curr = lcd->htim->Instance->CNT;
	volatile uint32_t stop = curr + delay_usec;

	int32_t diff = stop - curr;

	while (diff > 0)
	{
		curr = lcd->htim->Instance->CNT;
		diff = stop - curr;
	}
}


void TextLCD_Strobe(TextLCDType *lcd)
{
	// 0x0E == b# 0000 1110

	// Set bit 2 which corresponds to E (strobe) and send data
	lcd->data |= 0x04; // b# 0000 0100
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_udelay(lcd, STROBE_DELAY_USEC);
	// HAL_Delay(1);

	// Clear bit 2 which corresponds to E (strobe) and send data
	lcd->data &= 0xFB;	// b# 1111 1011
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_udelay(lcd, STROBE_DELAY_USEC);
	// HAL_Delay(1);
}

void TextLCD_Cmd(TextLCDType *lcd, uint8_t cmd)
{
	// cmd = 76543210
	// cmd & 0xF0 == cmd & (b# 1111 0000) == 7654____
	//       0x0C == b# 0000 1100

	// for command RS is set to 0
	lcd->data = (lcd->data & 0x0C) | (cmd & 0xF0);
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
	lcd->data = (lcd->data & 0x0C) | (cmd << 4);
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
}


void TextLCD_Data(TextLCDType *lcd, uint8_t data)
{
	// for data RS is set to 1
	lcd->data = (lcd->data & 0x0D) | (data & 0xF0) | 0x01;
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
	lcd->data = (lcd->data & 0x0D) | (data << 4) | 0x01;
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
}


/**
  *	Initialize the LCD to 4-bit mode.
  *	Set up 2 lines, 5x8 fonts and no cursor blinking.
  *	https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
  *	Figure 24 (initialize 4-bit mode)
  */
void TextLCD_Init(TextLCDType *lcd, I2C_HandleTypeDef *hi2c, uint8_t DevAddress, TIM_HandleTypeDef* htim)
{
	lcd->hi2c = hi2c;
	lcd->DevAddress = DevAddress;
	lcd->htim = htim;
	lcd->data = 0x38; // b# 0011 1000

	HAL_Delay(40); // Minimum startup time

	// Do init setting LCD controller into 4-bit mode as per Figure 24
	// 1. Try to set 8-bit mode
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
	HAL_Delay(5);
	// 2. Try to set 8-bit mode
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);
	HAL_Delay(1);
	// 3. Try to set 8-bit mode
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);

	// At this point display is reset to default 8-bit mode
	// Now configure 4-bit mode
	lcd->data = 0x28;	// b# 0010 1000
	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->DevAddress, &lcd->data, 1, 1000);
	TextLCD_Strobe(lcd);

	// Finished setting up 4-bit mode. Let's configure display

	TextLCD_Cmd(lcd, 0x28); //N=1 (2 line), F=0 (5x8)
	TextLCD_Cmd(lcd, 0x08); //Display off, Cursor Off, Blink off
	TextLCD_Cmd(lcd, 0x01); //Clear

	HAL_Delay(5);

	TextLCD_Cmd(lcd, 0x06); //ID=1(increment), S=0 (no shift)
	TextLCD_Cmd(lcd, 0x0C); //Display on, Cursor Off, Blink off

	HAL_Delay(5);

#if 1
	// This code creates custom characters to be used when making a VO-meter on the display
	// A VO-meter shows the volume graphically. the characters are 0x01, 0x02, 0x03, 0x04 and 0x05.
	// By putting out these characters you can draw a 1, 2, 3, 4 and 5 lines.
	for (uint8_t j = 1; j < 6; j++) {
		for (uint8_t i = 0; i < 8; i++) {
			TextLCD_CustomChar(lcd, i + j * 8, (uint8_t) (0xff00 >> (j + 3)));
		}
	}
#endif
}

/**
 * Turns the LCD on or off. Also sets cursor visibility and cursor blinking.
 */
void TextLCD_OnOff(TextLCDType *lcd, int on)
{
	uint8_t cmd_bit  = 0x08;
	uint8_t disp_bit = on ? 0x04 : 0x00;
	uint8_t curs_bit = on ? 0x02 : 0x00;
	uint8_t blnk_bit = on ? 0x01 : 0x00;
	uint8_t cmd      = cmd_bit | disp_bit | curs_bit | blnk_bit;
	TextLCD_Cmd(lcd, cmd);
}

// void TextLCD_Cmd(TextLCDType *lcd, uint8_t cmd)
void TextLCD_Home(TextLCDType *lcd)
{
	TextLCD_Cmd(lcd, 0x02);
	HAL_Delay(2);
}

/**
 *	The space character (0x20) is put in all DDRAM addresses.
 *	Address counter is set to 0.
 */
void TextLCD_Clear(TextLCDType *lcd)
{
	TextLCD_Cmd(lcd, 0x01);
	HAL_Delay(2);
}

void TextLCD_Position(TextLCDType *lcd, int x, int y)
{
	// Do modulo 16 and 2 for x and y to ensure within display boundaries
	x = x % 16;
	y = y % 2;

	uint8_t cmd_bit = 0x80;
	uint8_t adr     = x + (0x40 * y);
	uint8_t cmd     = cmd_bit | adr;
	TextLCD_Cmd(lcd, cmd);
}

void TextLCD_Hello(TextLCDType *lcd)
{
	const char			*p_hello = "Hello, World!";
	const uint8_t set_adr_cmd_bm = 0x80;
	uint8_t 				 adr = 0x00;
	uint8_t					 cmd = set_adr_cmd_bm | (adr & 0x7F); // 0111 1111

	// 1. set DDRAM pointer to adr 00
	TextLCD_Cmd(lcd, cmd);

	// 2. loop
	while (*p_hello++)
	{
		// kod
	}
}


void TextLCD_Putchar(TextLCDType *lcd, uint8_t ascii_char)
{
	TextLCD_Data(lcd, ascii_char);
}

void TextLCD_Puts(TextLCDType *lcd, char *string)
{
	int i = 0;
	while (string[i])
	{
		TextLCD_Putchar(lcd, string[i]);
		i++;
	}
}

void TextLCD_CustomChar(TextLCDType *lcd, uint8_t adr, uint8_t data)
{
	uint8_t cmd  = 0x40 | (adr & 0x3f);
	TextLCD_Cmd(lcd, cmd);

	TextLCD_Data(lcd, data);
}

#if 0
void TextLCD_Printf(TextLCDType *lcd, char *message, …)
{

}
#endif
