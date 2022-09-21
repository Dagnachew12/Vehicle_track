#include "NXP/iolpc2138.h"
#include "interrupts.h"

//Some local function definitions

void DefVectISR(void);          //Default ISR for non-vectored IRQ
static void (* fiq_isr)(void);  //FIQ ISR (there can only be one FIQ)

void VIC_init(void)
{
  unsigned int *VectAddr = (unsigned int *)&VICVectAddr0; //define pointer to vector address
  unsigned int *VectCntl = (unsigned int *)&VICVectCntl0; //define pointer to int controller register address

  VICIntEnClear = 0xFFFFFFFF;    //Disable all interrupts
  
  VICSoftIntClear = 0xFFFFFFFF;  //Clear Software Interrupts
  
  VICVectAddr = 0;               //Write to VicVectAddr register
  
  VICIntSelect = 0;              //Set all interrupts to to IRQ
  
  VICDefVectAddr = 0;            //Set Default Vector Address to NULL
  
  for ( int i = 0; i < VIC_CHANNELS; i++ ) //Set all the vector addresses to NULL (disable all IRQ channels)
  {                                                    
    VectAddr[i] = 0x0;
    VectCntl[i] = 0x0;
  }
}

void install_IRQ(unsigned int IntNumber, void (*ISR)(void), unsigned int channel)
{
   unsigned int * VectAddr = (unsigned int *)&VICVectAddr0;
   unsigned int * VectCntl = (unsigned int *)&VICVectCntl0;

   VICIntEnClear = 1<<IntNumber;  //Disable Interrupt at the specified channel 
  
   if(VIC_CHANNELS > channel)     //Vectored IRQ
   {
     VectAddr[channel] = (unsigned int)ISR;  //Set interrupt Vector to ISR
     VectCntl[channel] = IntNumber | (1<<5); //Set Int Number and enable the channel
   }
   else   //Non-vectired IRQ
   {
     VICDefVectAddr = (unsigned int)DefVectISR;  //Install ISR for non vectored IRQ (default ISR)
   }

   VICIntEnable = 1 << IntNumber;  //Enable Interrupt
}

void install_FIQ(unsigned int IntNumber,   void (*ISR)(void))
{
  VICIntEnClear = 1<<IntNumber;   //Disable Interrupt
  VICIntSelect |= 1<<IntNumber;   //Set FIQ mode
  fiq_isr = ISR;                  //Set interrupt Vector
  VICIntEnable = 1 << IntNumber;  //Enable Interrupt
}

__irq __arm void IRQ_Handler(void)
{
  void (* IntVector)(void);

  IntVector = (void (*)(void)) VICVectAddr;    //Read Interrupt Vector
  (* IntVector)();                             //Call ISR
  
  VICVectAddr = 0;                    //Dummy write to Vector address register (Errata)
}


__fiq __arm void FIQ_Handler(void)
{
  (* fiq_isr)();                             //Call ISR
}


static void DefVectISR (void)   
{
   //Put Code of Non-vectored ISR here
  return;
}

