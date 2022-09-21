/*--------------------------------------------------------------
 File:      led.c
 Purpose:   To control the LED connected to P0.8
 Compiler:  IAR EW 5.5
 Processor: Philips LPC2148 with ARM7TDMI-s Core
 Note:      Refer to the simulation schematic to learn how the LED is connected
----------------------------------------------------------------*/

#include "NXP/iolpc2138.h"

#include "led.h"

// initialize led port (PIO)
void led_init() 
{
  IO0DIR_bit.P0_5 = 1;    // Make Port P1.29 output---this is where LED is connected
  IO0SET_bit.P0_5 = 1;    // turn off led  --- see schematic on Proteus how LED is connected
}


void led_on() 
{
  
 IO0CLR_bit.P0_5 = 1;    //turn on the LED (clear the led pin to logic '0')
  
}

void led_off() 
{
  IO0SET_bit.P0_3 = 1;    //turn off LED (set the led pin to logic '1')
}
void del(unsigned long a)
{
  while(--a!=0);
}


