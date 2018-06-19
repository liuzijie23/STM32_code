#include "key.h"
#include "delay.h"
#include "oled.h"
#include "printf.h"
#include <stm32f10x.h>                  // Device header

int w;
u16 temperature[2] = {35,25};
void Key_IO_Init(void)
{
	GPIO_InitTypeDef IO_Init;
	SystemInit();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);	
	IO_Init.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
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
			if (w == 0)	temperature[0]--;
			else temperature[0]++;
			if (temperature[0] > 50) temperature[0] = 50;
			if (temperature[0] < temperature[1]) temperature[0] = temperature[1];
			OLED_ShowNum(96,2,temperature[0],4,8);
			printf("... s1 pushed ...\r\n");
		}
		while(!res1);
	}
	if(res2==0)
	{
		delay_us(1000);
		if(res2==0)
		{
			w = !w;
			printf("... s2 pushed ...\r\n");
		}
		while(!res2);
	}
	if(res3==0)
	{
		delay_us(1000);
		if(res3==0)
		{
			if (w == 0)	temperature[1]--;
			else temperature[1]++; 
			if (temperature[1] > temperature[0]) temperature[1] = temperature[0];
			if (temperature[1] < 0) temperature[1] = 0;
			OLED_ShowNum(96,3,temperature[1],4,8);
			printf("... s3 pushed ...\r\n");
		}
		while(!res3);
	}
	if(res4==0)
	{
		delay_us(1000);
		if(res4==0)
		{
			temperature[0] = 35;
			temperature[1] = 25;
			OLED_ShowNum(96,2,temperature[0],4,8);
			OLED_ShowNum(96,3,temperature[1],4,8);
			printf("... s4 pushed ...\r\n");
		}
		while(!res4);
	}

}
