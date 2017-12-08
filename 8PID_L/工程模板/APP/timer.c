#include "stm32f10x.h"                  // Device header
#include "timer.h"
#include "adc.h"
#include "printf.h"
#include "gy61.h"
#include "math.h"
#include "pid.h"
#include "pwm.h"

#ifndef N
#define N 10
#endif

#ifndef M
#define M 2
#endif
#define pi 3.1415926 
float o=0.0;
extern u16 i;
extern vu16 AD_Value[N][M];
//extern vu16 After_filter[M];

void timer_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//	TIM_TimeBaseInitStructure.TIM_Period = 2000;
	TIM_TimeBaseInitStructure.TIM_Period = 200;//定时器自动重载周期值
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
//	u32 j;
	float jj;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
//		GPIO_Write(GPIOA,++i); //一秒定时器工作指示灯
//		k=adc_result();
//		USART_SendData(USART1,0x41);
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		jj=200*sin((o/8)*pi)+2040.0;
		o++;
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//转换结束标志位
//		printf("\r\n");
//		printf("x:%6.2f   y:%6.2f\r\n",N2D_x(adc_result(0)),N2D_y(adc_result(1)));//输出AD对应角度值
		pwm_x(PIDM1_PosLocCalc(jj));
		
	}
}
