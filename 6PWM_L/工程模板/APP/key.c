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
	IO_Init.GPIO_Pin = GPIO_Pin_13;
	IO_Init.GPIO_Mode = GPIO_Mode_IPU;//上拉输入模式
	IO_Init.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &IO_Init);
}

void key_pros()
{
	if(res==0)
	{
		delay_us(1000);
		if(res==0)
//			GPIO_Write(GPIOA,++i);	//按键工作指示灯
//			GPIO_Write(GPIOA,(u16)(0x00));
		TIM_SetCompare2(TIM4, (++i*100)%900);
		printf("%d\r\r",(i*100)%900);
		while(!res);
	}
}
