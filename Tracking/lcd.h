
#include "NXP/iolpc2148.h"

#define LCD_RS IO1PIN_bit.P1_24
#define LCD_E  IO1PIN_bit.P1_25

#define  ON    1
#define  OFF   0

void lcd_cmd(char cmd);
void lcd_data(char data);
void lcd_gotoXY(char row, char col);
void lcd_put(char character, char row, char col);
void lcd_Write(char *buffer,char row,char col);
void lcd_init(void);
void lcd_backlight(char option);
void lcd_clear();
void lcd_num(unsigned int value);