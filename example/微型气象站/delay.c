#include "stm32f10x.h"                  // Device header
#include "delay.h"

void delay_us(u32 i)
{
	u32 temp;
	SysTick->LOAD=9*i;
	SysTick->CTRL=0X01;
	SysTick->VAL=0;
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&(!(temp&(1<<16))));
	
	SysTick->CTRL=0;
	SysTick->VAL=0;
}
