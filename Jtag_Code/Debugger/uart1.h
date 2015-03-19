#include<avr/io.h>
void init();
void uart_tx(unsigned int);
unsigned int uart_rx(void);
void uart_st(unsigned char *);
void uart_init()
{
	UCSR1B|=(1<<TXEN)|(1<<RXEN);
	UBRR1L=103;
}
void uart_tx(unsigned int data)
{
	while(!(UCSR1A&(1<<UDRE)));//empty while loop for the register to get ready to accept the data
	{
		UDR1=data;//this statement is outside the while loop
	}
}
unsigned int uart_rx(void)
{
	while(!(UCSR1A&(1<<RXC)));
	{
		return UDR1;
	}
}
void uart_st(unsigned char *str)
{
	while(*str!='\0')
	{
		uart_tx(*str++);
	}
}

void uart_tx_num(int num)
{
unsigned char data[10];
sprintf(data,"%d",num);
uart_st(data);



}
	 
