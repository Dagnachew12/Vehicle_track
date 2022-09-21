#include "NXP/iolpc2148.h"  //for register definitions

//Some macros
#define BUTTON   IO0PIN_bit.P0_14   //button is connected on port 0 bit 14

#define PRESSED  1
#define RELEASED 0

#define ON  1
#define OFF 0

//Function Prototypes
void button_init(void);
void button_ISR (void);
void delay_one(void);