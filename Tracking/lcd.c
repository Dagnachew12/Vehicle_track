
	
#include "NXP/iolpc2148.h"
#include "lcd.h"

void delay(unsigned long a);

void lcd_cmd(char cmd)
{
   IO1CLR_bit.P1_24 = 1;    //command mode (RS = 0)
   
   IO1PIN = cmd<<16;        //send command
   
   IO1SET_bit.P1_25 = 1;    //Set the enable input (E)
   delay(10);
   
   IO1CLR_bit.P1_25 = 1;  //toggle the E input to latch the command to the LCD controller (falling edge)
   delay(100);            //delay a little for the LCD to settle
}

void lcd_data(char data)
{
   IO1SET_bit.P1_24 = 1;  //data mode (RS = 1)
   
   //FIO1MASK = 0xFF00FFFF;   //mask the unused bits of the PIO (to keep them unaffected)
   IO1PIN =  ((data<<16)|0x01000000);        //send data
   //FIO1MASK = 0xF0FFFFFF;   //unmask RS and E
   
   IO1SET_bit.P1_25 = 1;   //Set the enable input (E)
   delay(10);
   IO1CLR_bit.P1_25 = 1;   //toggle the E input to latch the command to the LCD controller (falling edge)
   delay(100);              //delay a little for the LCD to settle
}

void lcd_gotoXY(char row, char col)
{
  char address;
  
  if((row > 2)||(col>16)) return;     //address out of range, just return
  
  if(row == 1) address = 0x80;        //first line
  else address = 0xC0;                //second line
  
  lcd_cmd(address + col);         //set address (row and column) 
}

void lcd_put(char character, char row, char col)
{
   lcd_gotoXY(row,col);
   lcd_data(character);
}

   lcd_gotoXY(row,col);
   
   while(*buffer != 0)
   {
      lcd_data(*buffer++);
   }
}

void lcd_init(void)
{
   IO1DIR |= 0xFFFFFFFF;  //set direction to output
   IO0DIR |= 0x40400000;  //set P0_22 and P0_30 as output (LCD_RW and LCD_BKL resp)
      
   delay(160000);         //around 20ms
   
   IO0CLR_bit.P0_22 = 1;  //LCD write mode
   
   lcd_cmd(0x01);     //clear display
   delay(8000);           //around 1ms
   
   lcd_cmd(0x38);     //LCD function set, 8-bit mode, 5x7 font, 2 lines display
   delay(40000);          //around 5ms
   
   lcd_cmd(0x38);     //LCD function set, 8-bit mode, 5x7 font, 2 lines display
   delay(40000);          //around 5ms
   
   lcd_cmd(0x08);     //turn off LCD
   delay(40000);          //around 5ms
   
   lcd_cmd(0x0E);     //turn on LCD, cursor on, blink off
   delay(160000);         //around 20ms
      
   lcd_cmd(0x06);     //Entry mode set, auto-increment address when writing
   delay(40000);          //around 5ms
}

void lcd_backlight(char option)
{
   IO0DIR_bit.P0_30 = 1;
   
   if(option == ON)
      IO0SET_bit.P0_30 = 1;    //backlight ON
   
   else
      IO0CLR_bit.P0_30 = 1;    //backlight OFF
}

//private function for a simple delay
void delay(unsigned long a)
{
   while(--a != 0);
}

void lcd_clear()
{
  lcd_cmd(0x01);
  IO1CLR_bit.P1_24 = 0;    //command mode (RS = 0)  
   IO1SET_bit.P1_25 = 1;    //Set the enable input (E)
   IO1CLR_bit.P1_25 = 0;  //toggle the E input to latch the command to the LCD controller (falling edge)
   delay(100);            //del
}

void lcd_num(unsigned int value)
{
 unsigned int arr[10],val;
 
 for(val=1;val<5;val++)
 {
  arr[val] =value%10;
  value  =value/10;
 }
 
 for(val=4;val!=0;val--)					//sending data to LCD
 {
  lcd_data(arr[val]+48);
 }
}
