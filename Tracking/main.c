/*----------------------------------------------------------------------------
    File name   : main.c
    Description : demonstrates on how to set the processor to power-down (sleep)
                  state and use a button to wake up from power-down mode using external
                  interrupt, thereby conserving energy and boosting the system performance

    Procesor    : Philips LPC2148 MCU with ARM7TDMI-S core
   
  ECEg-4501 - Microcomputers and interfacing, lab-exercise II
 ----------------------------------------------------------------------------*/

#include <intrinsics.h>    //for low level functions (like interrupt enable/disable)
#include "system.h"
#include "interrupts.h"
#include "led.h"
#include "cw_button.h"
#include "gsm_gps.h"
#include "ccw_button.h"
#include "adc.h"
#include "timer.h"
#include "NXP/iolpc2148.h"
#include "lcd.h"
#include "ccw_button.h"
#include "speaker.h"


   

void delay_fun(unsigned long a)
{
  while(--a!=0);
}


void main()
{
   
   __disable_interrupt(); 
   
   clock_init();         
   VIC_init();             
   button_init();                            
   led_init();
   lcd_init();
   gsm_gps_init(); 
   button3_init();
   adc_init();
   PWM_init();
   speaker_init();
   
   
   __enable_interrupt();  
   
  send_string("VEHICLE DRIVING TRACKING AND SAFTY SYSTEM \r\n");
  send_string1("RECEIVER MODULE FROM REMOTE AREA \r\n");
  send_string1("===================================================\r\n");
  char data;
  unsigned short gps;
  unsigned short alcol;
  unsigned short temp;
  unsigned short speed;
  unsigned short Tf,Tc;
   
  lcd_Write("Vehicle Accident ",1,1);
  lcd_Write("Tracking and Driving",2,1);
  lcd_Write("  Safty  System",3,2);
  delay_fun(500000);
  buzzer_off();  
   while(1)
   {
     
    send_char('\r');
    send_char('\n');
    data = get_char();  
    send_char(data);       
    send_char1(data);
    gps=adc_read(1);
    alcol=adc_read(2);
    temp=adc_read(3);
    speed=adc_read(6);
    Tc=temp*150/(466);        //Temperature of the engine in celcius degree
    Tf=celi_to_fher(Tc);    // Temperature of the engine in fheranite degree
    send_char('\r');
    send_char('\n');
    send_string("GPS:");
    print_int(gps);
    send_char('\r');
    send_char('\n');
    send_string("Alcohol:");
    print_int(alcol);
    send_char('\r');
    send_char('\n');
    send_string("Temperature (C):");
    print_int(Tc);
    send_char('\r');
    send_char('\n');
    send_string("Temperature (F):");
    print_int(Tf);
    send_char('\r');
    send_char('\n');
    send_string("Speed:");
    print_int(speed);
    send_char('\r');
    send_char('\n');
    delay_fun(500000); 
    
   
    
    
   }
}


   
