#include "NXP/iolpc2148.h"
#include "interrupts.h"
#include "gsm_gps.h"
#include "timer.h"

void timer_ISR(void);       //timer ISR ... see definition below

static unsigned int count = 0;      //a local variable to hold interrupt count
unsigned int rps;                   //a global variable to hold motor rps 

void timer_init()
{
  T0PR  = 47;       //Prescaler for TIMER0 --- T0_CLK = PCLK/48 = 48/48(Timer0 period = 1us)
  T0MR0 = 10000;    //timer period (match value = 10000. 10000 count = 10ms)
  T0MCR = 0x3;      //reset the counter and generate interrupt on match 0
  
  install_IRQ(VIC_TIMER0, timer_ISR,0); //Install Interrupt as vectored IRQ on channel 0

  T0TCR = 1;         //Enable Timer (timer counter starts to count every 1us)
}

{
  PINSEL0_bit.P0_10 = 2; //P0_10 configured as capture input 0 of timer1 (CAP1.0)
   
  T1PR = 0;              //Prescaler for TMR0-- T0_CLK = PCLK/1
  
  T1CTCR = 0x01;         //counter mode; TC is incremented on RISING edge @ CAP1.0 (P0_10 pin)
  
  T1TCR = 1;             //Enable and start Timer1 as a counter
}

void PWM_init (void)
{
  PINSEL0_bit.P0_7=2;
  PINSEL0_bit.P0_9  = 2;
    
  PWMPR = 47;       
  
  PWMMR0 = PWM_FREQ;    //PWM freq = PWM_ClK/1000 = 1KHZ (see "timer.h")
  
  //PWMMR6 = PWM_FREQ/2;  //Duty cycle = 1/2 or 50%
  PWMMR6 = PWM_FREQ1/2;  //Duty cycle = 1/2 or 50%
  PWMMR2=PWM_FREQ2/2;
    
  PWMMCR |= (1<<1);     //set bit 1 of PWM match Control Register (reset when match)
  
  PWMPCR |= (1<<14);    //Set bit 14 of PWM Port Control Register (enable channel 6 output)
  
  PWMPCR |= (1<<10);    //Set bit 13 of PWM Port Control Register (enable channel 2 output)
  
  PWMTCR = 0x09;        //PWM mode enabled, start counter 
  
  //configure lower legs of the H-bridge as outputs
  IO1DIR_bit.P1_26 = 1;
  IO1DIR_bit.P1_27 = 1;
}

void PWM_start(char channel, unsigned int duty)
{
  if(channel == 6)
  {
     PWMLER = (1<<6);      //enable change of duty cycle
     PWMMR6 = duty;        //update Duty cycle
     PWMPCR |= (1<<14);    //Set bit 13 of PWM Port Control Register (enable channel 5 output)
  }
  
  else
  {
    
     PWMLER = (1<<2);      //enable change of duty cycle
     PWMMR2 = duty;        //update Duty cycle 
     PWMPCR |= (1<<10);    //Set bit 10 of PWM Port Control Register (enable channel 6 output)
  }
  }

void pwm_change2(unsigned int fact)
{
  PWMLER=(1<<2);
  PWMMR2=fact;
  
}

void pwm_change6(unsigned int fact)
{
  PWMLER=(1<<6);
  PWMMR6=fact;
  
}

void PWM_stop(char channel)
{
  char shift = channel + 8;
  
  PWMPCR &= ~(1<<shift);    //disable PWM on Port Control Register 
}

void PWM_changeDuty(char channel, unsigned int duty)
{
   if(channel == 6)
  {
     PWMLER = (1<<6);      //enable change of duty cycle
     PWMMR6 = duty;        //update Duty cycle
  }
  
  else
  {
    PWMLER = (1<<2);      //enable change of duty cycle
     PWMMR2 = duty;
  }
}

#pragma section = ".irqisr"           //in the IRQ vector address
void timer_ISR(void) @ ".irqisr"
{
  
  T0IR = 1;     //Clear Interrupt flag (otherwise interrupt won't occur again)   
   
  count++;     //increment the interrupt count
                 
  if(count == 100)   //1 second elapsed? (100 occurences = 1sec, since interrupt occurs every 10ms)
  {
      count = 0;    //reset interrupt count
     
      //Now update the motor revolution per second
      rps = T1TC;
      
      //reset the revolution counter
      T1TC=0;
  }  
}
   
  
