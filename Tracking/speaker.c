#include "NXP/iolpc2148.h"
#include <stdio.h>
#include "speaker.h"

void speaker_init()

{
IO0DIR_bit.P0_2 = 1;
IO0SET_bit.P0_2=1;
}
void buzzer_on(void)
{
IO0CLR_bit.P0_2 = 1;

}

void buzzer_off(void)
{
IO0SET_bit.P0_2=1;
}



