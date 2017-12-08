#include "stm32f10x.h"                  // Device header
#include "timer.h"

extern u16 i;

void timer_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_TimeBaseInitStructure.TIM_Period = 2000;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 35999;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM3,ENABLE);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		GPIO_Write(GPIOA,++i);
	}
}
