#include "key.h"
#include "delay.h"
#include "printf.h"
#include <stm32f10x.h>                  // Device header

extern u16 i;
void Key_IO_Init(void)
{
	GPIO_InitTypeDef IO_Init;
	SystemInit();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);	
	IO_Init.GPIO_Pin = GPIO_Pin_0;
	IO_Init.GPIO_Mode = GPIO_Mode_Out_PP;
	IO_Init.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &IO_Init);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	
	IO_Init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	IO_Init.GPIO_Mode = GPIO_Mode_IPU;//上拉输入模式
	IO_Init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &IO_Init);
}

void key_pros()
{
	if(res1==0)
	{
		delay_us(1000);
		if(res1==0)
		{	
			printf("... s1 pushed ...\r\n");
			//delay_us(1000);
			//DHT11_get_data();
		}
		while(!res1);
	}
	if(res2==0)
	{
		delay_us(1000);
		if(res2==0)
		printf("... s2 pushed ...\r\n");
		while(!res2);
	}
	if(res3==0)
	{
		delay_us(1000);
		if(res3==0)
		printf("... s3 pushed ...\r\n");
		while(!res3);
	}
	if(res4==0)
	{
		delay_us(1000);
		if(res4==0)
		printf("... s4 pushed ...\r\n");
		while(!res4);
	}
}
