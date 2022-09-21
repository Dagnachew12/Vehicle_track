#include "NXP/iolpc2148.h"  //for register definitions

#define BUTTON2  IO0PIN_bit.P0_7;   //button is connected on port 0 bit 14

#define PRESSED  1
#define RELEASED 0

#define ON  1
#define OFF 0

//Function Prototypes
void button3_init(void);
void button3_ISR (void);
void delay1();
