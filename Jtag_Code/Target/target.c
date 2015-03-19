#include<avr/io.h>
 #include<util/delay.h>
 void jtag_init()
{
//DDRC=0xff;
DDRF|=(1<<6);// Set TDO as output
DDRF&=~((1<<7)|(1<<5)|(1<<4));// Set TDI,TMS,TCK as input.
//PORTF|=(1<<6)|(1<<5);
//uart_init();
}
 
 void main()
 {
 jtag_init();
 
 DDRC=0xff;
 DDRG=0xff;
 PORTG=0xff;
 PORTC=0xff;
 DDRB&=~(1<<7);
 PORTB|=(1<<7);
 while(1)
 {
 if(PINB&(1<<7))
 PORTC=0xff;
 else
 PORTC=0x00;
 }
 }
