#include "NXP/iolpc2148.h"
#include "interrupts.h"
#include "led.h"
#include "gsm_gps.h"
#include "timer.h"
#include "adc.h"
#include "speaker.h"
#include "lcd.h"
   
   unsigned char gps_data1[]={'2','5','7','7','3','9','.','1','4','4',
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


void button3_ISR(void);    //function prototype - see definition below

//static char flag = 0;     //static global variable to hold LED state
unsigned short gps;

/*-------------------------------------------------------------------------
   Function Name: button_init

   Parameters: None
 
   Return:  None
 
   Description: Initializes the button as falling edge triggered
                external interrupt source on the MCU
                (refer cha-4 "System control" - 4.5 External interrupt inputs)
 ---------------------------------------------------------------------------*/

void delay1()
{
  for(int i=0;i<1000;i++)
    for (int j=0;j<100;j++);
}

void button3_init(void)
{
   PINSEL1_bit.P0_20=0x3;  //select external interrupt function on pin P0.14 (EINT 1)
  
   EXTINT = 0x0F;        //clear any pending external interrupt requests
   
   EXTMODE |=0x3;       //external interrupt 1 is edge triggered
  
   EXTPOLAR =0x00;      //and falling edge is selected
  
   EXTWAKE |= 0x3;       //configure external interrupt on P0.14 to wake up the MCU from sleep
  
   install_FIQ(VIC_EINT3, button3_ISR); //Install Interrupt as IRQ
}

/*-------------------------------------------------------------------------
   Function Name: button_ISR

   Parameters: None
 
   Return    :  None
 
   Description: An interrupt service routine (ISR) to handle the external interrupt
                request by the button
 ---------------------------------------------------------------------------*/
#pragma section = ".fiqisr"      //locate code below on the FIQ mode stack

void button3_ISR(void) @ ".fiqisr"
{ 
  while(EXTINT_bit.EINT3)
   {
      EXTINT_bit.EINT3 = 1;      
   }
  while(1)
  {
  unsigned short alcohol_amount=adc_read(2);
  unsigned short gpsr_data=adc_read(6);
  
  if(alcohol_amount<400)
  {
      send_string("Drive safelly!\r\n");
      unsigned short speed_ch=10000*adc_read(1)/(1023);
      led_off();
      buzzer_off();
      PWM_start(2,speed_ch);
      PWM_changeDuty(2,speed_ch);
      pwm_change2(speed_ch);
      IO1SET_bit.P1_27=1;
      IO1CLR_bit.P1_26=1;
      PWM_stop(6); 
      send_string("The motor runs counter clockwise direction.\r\n");
  }
  else
  {
     buzzer_on();
     led_on();
    PWM_stop(2);
    PWM_stop(6);
    IO1CLR_bit.P1_26=1;
    IO1CLR_bit.P1_27=1;
     delay1();
     lcd_cmd(0x01);
     lcd_Write("  Starting  GSM...",1,1);
     sendsms();
     delay1();
     send_location(gps_data1,gpsr_data);
     delay1();
     delay1();
     lcd_Write("   Message Sent!",2,1);
     delay1();
       delay1();
         delay1();
          delay1();
            delay1();
              delay1();
                delay1();
   
  }
  }
   
}


