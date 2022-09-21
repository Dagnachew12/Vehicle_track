#include "NXP/iolpc2148.h"  //for register definitions
#include "adc.h"
#include "gsm_gps.h"
#include <stdio.h>
#include <math.h>

extern unsigned char at[]="AT";
extern unsigned char ate[]="ATe0";
extern unsigned char cmgf[]="AT+CMGF=1";                            //Text format in GSM modem
extern unsigned char cmgs[]="AT+CMGS=\"+251935717907\"";               //Mobile number to which the msg is sent
extern unsigned char msg[]="some one is traying to drive the car after drinking!";                                //secret code
extern unsigned char readall[]="AT+CMGR=\"REC UNREAD\"\r\n";
//static unsigned char lat[10];
//static unsigned char lon[11];
static unsigned char latitude[10];
static unsigned char longitude[10];



void gsm_gps_init(void)
{
  PINSEL0_bit.P0_0 = 1;     //select pin function as uart (chapter6-pin connect block) -- TXD
  PINSEL0_bit.P0_1 = 1;     //select pin function as uart (chapter6-pin connect block) -- RXD
  PINSEL0_bit.P0_8 = 1;     //select pin function as uart (chapter6-pin connect block) -- TXD
  PINSEL0_bit.P0_9 = 1;     //select pin function as uart (chapter6-pin connect block) -- RXD
  
  U0LCR_bit.DLAB = 1;       //Divisor latch enabled (reqired only during configuration)
  
  U0DLM = 0;                
  U0DLL = 156;              //Baud rate dividing ratio = 156 (for 19200bps - see manual on how to calculate)
  
  U0FDR_bit.MULVAL = 1;     //Fractional baud rate multiplier = 1  (optional)
  U0FDR_bit.DIVADDVAL=0;    //Fractional baud rae divider = 0+1    (optional)
  
  U0FCR_bit.FCRFE = 1;      //RX and TX FIFO enabled
  U0LCR |= 0x03;            //8bits data, no parity, 1 stop bit
  U0TER_bit.TxEn = 1;       //Enable Transmitter
  
  U0LCR_bit.DLAB = 0;       //Divisor latch disabled (no more needed)
  U0FCR |= 0x01;                                            // FIF0 Enable
  U0FCR |= 0x02;                                            // RX FIFO Reset
  U0FCR |= 0x04;                                            // TX FIFO Reset
  U0FCR &= 0x3F;

  
  U1LCR_bit.DLAB = 1;       //Divisor latch enabled (reqired only during configuration)
  
  U1DLM = 0;                
  U1DLL = 156;              //Baud rate dividing ratio = 156 (for 19200bps - see manual on how to calculate)
  
  U1FDR_bit.MULVAL = 1;     //Fractional baud rate multiplier = 1  (optional)
  U1FDR_bit.DIVADDVAL=0;    //Fractional baud rae divider = 0+1    (optional)
  
  U1FCR_bit.FCRFE = 1;      //RX and TX FIFO enabled
  U1LCR |= 0x03;            //8bits data, no parity, 1 stop bit
  U1TER_bit.TxEn = 1;       //Enable Transmitter
  
  U1LCR_bit.DLAB = 0;       //Divisor latch disabled (no more needed)
  
  U1FCR |= 0x01;                                            // FIF0 Enable
  U1FCR |= 0x02;                                            // RX FIFO Reset
  U1FCR |= 0x04;                                            // TX FIFO Reset
  U1FCR &= 0x3F; 
}

void send_char(char data)
{
  while(U0LSR_bit.TEMT ==0);      //Wait for previous transmission to finish
  U0THR = data;                   //Shift out the data serially on the TX line
}

void send_char1(char data)
{
  while(U1LSR_bit.TEMT ==0);      //Wait for previous transmission to finish
  U1THR = data;                   //Shift out the data serially on the TX line
}

void send_string(char *str)
{
 
  while(*str != 0)
  {
    while(U0LSR_bit.TEMT ==0);      //Wait for previous transmission to finish
    U0THR = *str++;              //Shift out the data serially on the TX line
  }
}

void send_string1(char *str)
{
 
  while(*str != 0)
  {
    while(U1LSR_bit.TEMT ==0);      //Wait for previous transmission to finish
    U1THR = *str++;              //Shift out the data serially on the TX line
  }
}

char get_char(void)
{
  char data;                     //variable to hold incoming byte 
  
  while(U0LSR_bit.DR == 0);     //wait for data to come in
  data = U0RBR;                 //copy byte from the receive register to a variable
  return data;                  //return data received to the calling program
}


char get_char1(void)
{
  char data;                     //variable to hold incoming byte 
  
  while(U1LSR_bit.DR == 0);     //wait for data to come in
  data = U1RBR;                 //copy byte from the receive register to a variable
  return data;                  //return data received to the calling program
}

void get_string(char *buffer, unsigned short length)
{
  for(int i=0; i < length; i++)
  {
    while(U0LSR_bit.DR == 0);    //wait for data to come in
    *buffer++ = U0RBR;           //copy byte from the receive register to buffer
  }
}

void get_string1(char *buffer, unsigned short length)
{
  for(int i=0; i < length; i++)
  {
    while(U1LSR_bit.DR == 0);    //wait for data to come in
    *buffer++ = U1RBR;           //copy byte from the receive register to buffer
  }
}

void print_int(unsigned int integer)
{
   char digit[4];
   unsigned int temp = 0;
   
   digit[0] = integer/1000;   //first digit
   
   temp = integer % 1000;     //temp = the remainder after dividing by 100
   
   digit[1] = temp/100;       //the second digit
   
   temp = temp % 100;     //third digit
   digit[2]=temp/10;
   temp=temp%10;
   digit[3]=temp;
   
   //change digits in to ASCII and print (add 0x30 or 48 to change to ASCII)
   for(int i=0; i<4; i++)
   {
      send_char(digit[i]+48);
      //send_char1(digit[i] +48);
   }
}


void sendstring(unsigned char *p)            //Sends a string of data through UART1
{
while(1)
{
if(*p=='\0') break;
send_char(*p++);
}
}


void sendstring1(unsigned char *p)            //Sends a string of data through UART1
{
while(1)
{
if(*p=='\0') break;
send_char1(*p++);
}
}


/*
void receive_gps(void)
{
unsigned char p;
int i;
//j;



int gps=0;

while(gps==0)
{
p=get_char();
while(p!='$')
{
p=get_char();
}
p=get_char();
if(p=='G')
{
}
else
continue;
p=get_char();
if(p=='P')
{
}
else
continue;
p=get_char();
if(p=='G')
{
}
else
continue;
p=get_char();
if(p=='G')
{
}
else
continue;
p=get_char();
if(p=='A')
{
while(p!=',')
{
p=get_char();

}
p=get_char();
while(p!=',')
{
p=get_char();
}
for(i=0;i<9;i++)
{
lat[i]=get_char();
}
p=get_char();
while(p!=',')
{
p=get_char();
}
p=get_char();
while(p!=',')
{
p=get_char();
}
for(i=0;i<10;i++)
{
lon[i]=get_char();
}
lat[9]='\0';
lon[10]='\0';
gps=1;
}
else
continue;
}


}
*/


void delayfunc()                              //delay function
{
int i,j;
for(i=0;i<1000;i++)
for(j=0;j<100;j++);
}

void sendsms(void)
{
send_char1('\r');
send_char1('\n'); 
sendstring(at);
send_char(0x0d);                                     // equivalent of 
send_char(0x0a);
send_char1('\r');
send_char1('\n');        
delayfunc();
sendstring(ate);
send_char(0x0d);                                     // equivalent of 
send_char(0x0a);                                     //   enter key
delayfunc();
send_char1('\r');
send_char1('\n'); 
sendstring(cmgf);
send_char(0x0d);                                     // equivalent of 
send_char(0x0a);                                     //   enter key
delayfunc();
send_char1('\r');
send_char1('\n'); 
sendstring(cmgs);
send_char(0x0d);
send_char1(0x0a);  
delayfunc();
delayfunc();
send_char1('\r');
send_char1('\n'); 
sendstring1(msg);
delayfunc();
send_char1(0x1a);
delayfunc();
send_char1(0x1a);
send_char1('\r');
send_char1('\n'); 

}


void send_location(unsigned char *gps,unsigned short gps_dat)
{
  
   
   if(gps_dat<100)
   {
     
  send_string1("Longitude:");
  for(int i=0;i<10;i++)
  {
    longitude[i]=gps[i];
    send_char1(longitude[i]);
  }
               
 send_char1('\r');
 send_char1('\n');
 send_string1("Latitude:");
 for( int i=10;i<20;i++)
 {
   latitude[i]=gps[i];
   send_char1(latitude[i]);
 }
 send_char1('\r');
 send_char1('\n');
}

else if(gps_dat>100 && gps_dat<200)
{
  send_string1("Longitude:");
  for(int i=20;i<30;i++)
  {
    longitude[i]=gps[i];
    send_char1(longitude[i]);
  }
               
 send_char1('\r');
 send_char1('\n');
 send_string1("Latitude:");
 for( int i=30;i<40;i++)
 {
   latitude[i]=gps[i];
   send_char1(latitude[i]);
 }
 send_char1('\r');
 send_char1('\n');
}

else if(gps_dat>200 & gps_dat<300)
{
  send_string1("Longitude:");
  for(int i=40;i<50;i++)
  {
    longitude[i]=gps[i];
    send_char1(longitude[i]);
  }
               
 send_char1('\r');
 send_char1('\n');
 send_string1("Latitude:");
 for( int i=50;i<60;i++)
 {
   latitude[i]=gps[i];
   send_char1(latitude[i]);
 }
 send_char1('\r');
 send_char1('\n');
}
else if(gps_dat>300 && gps_dat<400)
{
  send_string1("Longitude:");
  for(int i=60;i<70;i++)
  {
    longitude[i]=gps[i];
    send_char1(longitude[i]);
  }
               
 send_char1('\r');
 send_char1('\n');
 send_string1("Latitude:");
 for( int i=70;i<80;i++)
 {
   latitude[i]=gps[i];
   send_char1(latitude[i]);
 }
 send_char1('\r');
 send_char1('\n');
}
else if(gps_dat>400 && gps_dat<500)
{
  send_string1("Longitude:");
  for(int i=80;i<90;i++)
  {
    longitude[i]=gps[i];
    send_char1(longitude[i]);
  }
               
 send_char1('\r');
 send_char1('\n');
 send_string1("Latitude:");
 for( int i=90;i<100;i++)
 {
   latitude[i]=gps[i];
   send_char1(latitude[i]);
 }
 send_char1('\r');
 send_char1('\n');
}
else if(gps_dat>500 && gps_dat<600)
{
  send_string1("Longitude:");
  for(int i=100;i<110;i++)
  {
    longitude[i]=gps[i];
    send_char1(longitude[i]);
  }
               
 send_char1('\r');
 send_char1('\n');
 send_string1("Latitude:");
 for( int i=110;i<120;i++)
 {
   latitude[i]=gps[i];
   send_char1(latitude[i]);
 }
 send_char1('\r');
 send_char1('\n');
}
else if(gps_dat>600 && gps_dat<700)
{
  send_string1("Longitude:");
  for(int i=120;i<130;i++)
  {
    longitude[i]=gps[i];
    send_char1(longitude[i]);
  }
               
 send_char1('\r');
 send_char1('\n');
 send_string1("Latitude:");
 for( int i=130;i<140;i++)
 {
   latitude[i]=gps[i];
   send_char1(latitude[i]);
 }
 send_char1('\r');
 send_char1('\n');
}
else if(gps_dat>700 && gps_dat<800)
{
  send_string1("Longitude:");
  for(int i=140;i<150;i++)
  {
    longitude[i]=gps[i];
    send_char1(longitude[i]);
  }
               
 send_char1('\r');
 send_char1('\n');
 send_string1("Latitude:");
 for( int i=150;i<160;i++)
 {
   latitude[i]=gps[i];
   send_char1(latitude[i]);
 }
 send_char1('\r');
 send_char1('\n');
}
else if(gps_dat>800 && gps_dat<900)
{
  send_string1("Longitude:");
  for(int i=160;i<170;i++)
  {
    longitude[i]=gps[i];
    send_char1(longitude[i]);
  }
               
 send_char1('\r');
 send_char1('\n');
 send_string1("Latitude:");
 for( int i=170;i<180;i++)
 {
   latitude[i]=gps[i];
   send_char1(latitude[i]);
 }
 send_char1('\r');
 send_char1('\n');
}
else if(gps_dat>900 && gps_dat<1023)
{
  send_string1("Longitude:");
  for(int i=180;i<190;i++)
  {
    longitude[i]=gps[i];
    send_char1(longitude[i]);
  }
               
 send_char1('\r');
 send_char1('\n');
 send_string1("Latitude:");
 for( int i=190;i<200;i++)
 {
   latitude[i]=gps[i];
   send_char1(latitude[i]);
 }
 send_char('\r');
 send_char('\n');
}

  
  }
