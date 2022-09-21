/*--------------------------------------------------------------
 File:      led.h
 Purpose:   header file for the led driver
 Compiler:  IAR EW 5.5
 MCU:       Philips LPC2148 with ARM7TDMI-s Core
----------------------------------------------------------------*/

/*---- Function prototypes ------- */

// init led port
void led_init(void);

// turn on led 
void led_on(void);

// turn off led 
void led_off(void);

void del(unsigned long a);

