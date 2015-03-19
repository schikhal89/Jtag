#include<avr/io.h>
#include<util/delay.h>
#include"jtag.h"

void main()
{
uart_init();
//uart_tx_num(123);
jtag_init();

scanio(1);
while(1);

}
