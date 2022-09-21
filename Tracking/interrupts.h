#ifndef   __INTERRUPTS_H
#define   __INTERRUPTS_H

//Macros
#define INT_NUMBERS   32   //Total Interrupt Sources for ARM7
#define VIC_CHANNELS  16   //IRQ Priority Leves

//Function Prototypes
void VIC_init(void);

void install_IRQ(unsigned int IntNumber,  void (*ISR)(void), unsigned int channel);

void install_FIQ(unsigned int IntNumber,  void (*ISR)(void));

#endif //__INTERRUPTS_H
