#include "stm32f10x.h"
#include "pwm.h"
void pwm_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//GPIO_Mode_AF_PP
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period = 17999;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, & TIM_TimeBaseInitStructure);
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM4,ENABLE);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_SetCompare2(TIM4, 0);
	TIM_Cmd(TIM4,ENABLE);
}
//TIM_SetCompare2(TIM4, ti);
