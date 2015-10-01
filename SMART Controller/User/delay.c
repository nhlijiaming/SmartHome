#include "stm32f10x.h"
#include "delay.h"

extern u32 TimingDelay;
void delay_us(u32 nTime)
{
    TimingDelay = nTime;
    while (TimingDelay) ;
}
void delay_ms(u32 nTime)
{
	while(nTime--)
	{
		delay_us(1000);		
	}
}
