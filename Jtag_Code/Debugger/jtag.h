#include"uart1.h"

#include<util/delay.h>
#define TMS 5
#define TDI 7
#define TDO 6
#define TCK 4
#define readb(x,a) x>>1&a
#define clrb(x,a) x|=(1<<a)
#define setb(x,a) x&=~(1<<a)
#define F PORTF


unsigned long long TDO_val;

void jtag_init()
{
DDRF|=(1<<5)|(1<<7)|(1<<4);
DDRF&=~(1<<6);
}


char jtag_io(char tms, char tdi)
{
char value;
clrb(F,TCK);
_delay_us(1);
setb(F,TCK);
if(tms==1)
setb(F,TMS);
else
clrb(F,TMS);
if(tdi==1)
setb(F,TDI);
else
clrb(F,TDI);

setb(F,TCK);
_delay_us(1);
clrb(F,TCK);
_delay_us(1);


value=readb(PINF,TDO);
}



void irscan(int count, char *tdi)
{
int tdo_bit;
int i;
jtag_io(1,0);
jtag_io(1,0);
jtag_io(0,0);
jtag_io(0,0);
clrb(F,TCK);

for( i=0;i<count;i++)
{
tdo_bit=jtag_io((i==count-1), tdi[i]);
TDO_val=(unsigned long long)(tdo_bit&1)<<count;
TDO_val=TDO_val>>1;
}
jtag_io(1,0);
jtag_io(0,0);
jtag_io(1,0);
jtag_io(1,0);
jtag_io(0,0);
}

void drscan(int count, char *tdi, int bit_section)
{
int i;
if(bit_section==1)
i=0;
if(bit_section==2)
{
i=64;
count=count+i;
}
if(bit_section==3)
{
i=128;
count=count+i;
}
if(bit_section==4)
{
i=192;
count+count+i;
}

int tdo;
jtag_io(1,0);
jtag_io(1,0);
jtag_io(1,0);

clrb(F,TCK);
tdo=readb(PINF,4);
TDO_val=(unsigned long long)(tdo&1)<<count+1;
for(i=0;i<count;i++)
{
tdo=jtag_io((i==count), tdi[i]);
TDO_val=(unsigned long long)tdo&1<<count-1;
TDO_val=TDO_val>>1;
}
jtag_io(1,0);
jtag_io(0,0);
jtag_io(1,0);
jtag_io(1,0);
jtag_io(0,0);
}

void printTDO_val(void)
{
unsigned int lowest;
unsigned int low;
unsigned int high;
unsigned int highest;

lowest=(unsigned int)TDO_val;
low=(unsigned int)TDO_val>>16;
high=(unsigned int)TDO_val>>32;
highest=(unsigned int)TDO_val>>64;

uart_tx_num(lowest);
uart_tx_num(low);
uart_tx_num(high);
uart_tx_num(highest);
}

void scanio(int bit_section)
{
char sample_preload[4]={0,1,0,0};
char extest[4]={0,0,0,0};

char sample[64]={
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0};

irscan(4,sample_preload);
drscan(64,sample,bit_section);

irscan(4,extest);
drscan(64,sample,bit_section);
printTDO_val();
}


