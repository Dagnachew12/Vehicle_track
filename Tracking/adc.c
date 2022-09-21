#include "NXP/iolpc2148.h"
#include "adc.h"




void adc_init()
{
   PINSEL0_bit.P0_4 = 1; //set bit 4 PIO0 as analog input (channel-6)
   PINSEL1_bit.P0_28 = 1; //set bit 28 PIO0 as analog input (channel-1)
   PINSEL1_bit.P0_29 = 1; //set bit 29 PIO0 as analog input (channel-2)
   PINSEL1_bit.P0_30=1;   //set bit 30 as analoge input(channel-3)

         
   AD0CR_bit.SEL=0x4E;  //Conversion to be done on channel-1 , channel-2 ,channel-3 and channel-6
   
   AD0CR_bit.CLKDIV = 11; //ADC_CLK = PCLK/12 = 48M/12 = 4M (Note: ADC_CLK must be <= 4.5MHz)
   
   AD0CR_bit.START = 0;   //don't start conversion yet
    
   AD0CR_bit.PDN = 1;     //AD converter is enabled and is ready 
}

/*void Serial_Init ()
{
PINSEL0 |= 0x00050000; // TxD1 and RxD1 @ P0.8 & P0.9
U1LCR = 0x83;
U1DLL = 195;
U1LCR = 0x03;
}*/

unsigned short adc_read(char channel)
{
   unsigned short digital_value;
   
   AD0CR_bit.SEL = (1<<channel);    //Conversion to be done on selected channel
   
   AD0GDR_bit.DONE = 0;             //to be sure previous result was read (optional)
      
   AD0CR_bit.START = 1;                //start conversion now
      
   while(!AD0GDR_bit.DONE);            //wait for conversion to complete (DONE bit becomes 1 when conversion is done)
   
   AD0CR_bit.START = 0;                //clear the start bit
   
   digital_value = AD0GDR_bit.RESULT;  //read the 10-bit digital conversion result
   
   return digital_value;               //return digital value to the calling program
}

unsigned short celi_to_fher(unsigned short Tc)
   {
     unsigned short Tf;
     Tf=9*Tc/(5) + 32;
     return Tf;
   }