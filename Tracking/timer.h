//1KHZ @ PCLK = 12MHz
#define PWM_FREQ 1000 
#define PWM_FREQ1 1000 
#define PWM_FREQ2 1000 

//motor current return path
#define LEG1_ON   IO1SET_bit.P1_26
#define LEG1_OFF  IO1CLR_bit.P1_26
#define LEG2_ON   IO1SET_bit.P1_27
#define LEG2_OFF  IO1CLR_bit.P1_27

void timer_init(void);
void counter_init(void);
void PWM_init(void);
void PWM_start(char channel, unsigned int duty);
void PWM_stop(char channel);
void PWM_changeDuty(char channel, unsigned int duty);
void pwm_change2(unsigned int fact);
void pwm_change6(unsigned int fact);
