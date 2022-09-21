#include "NXP/iolpc2148.h"
#include "interrupts.h"
#include "led.h"
#include "gsm_gps.h"
#include "timer.h"
#include "adc.h"
#include "speaker.h"
#include "lcd.h"
   
   
   unsigned char gps_data[]={'2','5','7','7','3','9','.','1','4','4',
                      '6','6', '3', '4', '6', '2', '.','8','5','0', 
                      '6', '1', '2', '4', '8', '7', '.','2','7','9', 
                      '7', '6', '1',  '5', '8', '4', '.','2','0','1',
                      '3', '0', '9', '1',  '9', '1', '.','7','2','4',
                      '8', '9', '3', '0', '7', '5', '.','9','1','1', 
                      '3', '6', '2', '0', '9', '2', '.','6','0','6',
                      '4', '5', '6', '9', '8', '2', '.', '5','6','7',
                      '8','6','8','5','6','3','.','8','2','2',
                      '7','0','4','7','4','2','.','7','4','8',
                        
                       '1','5','7','8','3','9','.','1','6','4',
                       '6','0', '3', '4', '7', '2', '.','9','5','0', 
                      '9', '1', '2', '4', '8', '7', '.','2','7','9', 
                      '8', '6', '1',  '5', '8', '4', '.','2','7','1',
                      '3', '0', '9', '1',  '9', '1', '.','7','3','4',
                      '2', '9', '3', '0', '7', '5', '.','9','7','1', 
                      '7', '6', '2', '0', '9', '2', '.','6','5','6',
                      '6', '5', '6', '9', '8', '2', '.', '5','6','7',
                      '1','6','8','5','6','3','.','8','0','2',
                      '2','0','4','7','4','2','.','7','0','8'
                      };

void button_ISR(void);    //function prototype - see definition below

//static char flag = 0;     //static global variable to hold LED state
unsigned short duty;

void delay_one(unsigned long a)
{
  while(--a!=0);

}
void button_init(void)
{
   PINSEL0_bit.P0_14 = 0x2;  //select external interrupt function on pin P0.14 (EINT 1)
  
   EXTINT = 0x0F;        //clear any pending external interrupt requests
   
   EXTMODE = 0x02;       //external interrupt 1 is edge triggered
  
   EXTPOLAR = 0x00;      //and falling edge is selected
  
   EXTWAKE = 0x02;       //configure external interrupt on P0.14 to wake up the MCU from sleep
  
   install_IRQ(VIC_EINT1, button_ISR,1); //Install Interrupt as FIQ
}

void delay_int(unsigned long d)
{
  while(--d!=0);
}
#pragma section = ".fiqisr"      //locate code below on the FIQ mode stack

void button_ISR(void) @ ".fiqisr"
{  
  while(EXTINT_bit.EINT1)
   {
      EXTINT_bit.EINT1 = 1;      
   }
   while(1)
   {
     unsigned short alcol_amt=adc_read(2);
     unsigned short gpst_data=adc_read(1);
     
   if(alcol_amt<400)
   {
      unsigned short speed_cha=10000*adc_read(1)/(1023);
      led_off();
      buzzer_off();
      send_string("Well come you derive safe ! \r\n");
      PWM_start(6,speed_cha);
      PWM_changeDuty(6,speed_cha);
      pwm_change6(speed_cha);
      IO1SET_bit.P1_26=1;
      IO1CLR_bit.P1_27=1;
      PWM_stop(2);
      send_string("The motor runs clockwise direction.\r\n");
       
   }
   
   else
   {
    buzzer_on();
    led_on();
    PWM_stop(2);
    PWM_stop(6);
    IO1CLR_bit.P1_26=1;
    IO1CLR_bit.P1_27=1;
    
     delay_one(500000);
     lcd_cmd(0x01);
     lcd_Write("  Starting  GSM.....",1,1);
     sendsms();
     delay_one(100000);
     send_location(gps_data,gpst_data);
     delay_one(500000);
     lcd_Write("  Message Sent!",2,1);
     delay_one(100000000);
    


   }

   }
   
 
}



