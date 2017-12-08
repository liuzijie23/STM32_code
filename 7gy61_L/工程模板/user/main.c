#include <stm32f10x.h>
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "adc.h"
//#include "public.h"
#include "timer.h"
#include "printf.h"
#include "pwm.h"
#include "key.h"
#include "gy61.h"
#include "stm32f10x.h"

#define N 10
#define M 2
float Du_x,Du_y;//定义x方向偏角、y方向偏角

vu16 AD_Value[N][M];
//vu16 After_filter[M];
u16 i=0;
int main()
{
	pwm_init();
	usart_init();
	printf_init();
	adc_init();
	Key_IO_Init();
	timer_init();
//	GPIO_Write(GPIOA,(u16)0x00);
	printf("...start...\r\n");
	while(1)
	{
		key_pros();
		delay_us(2000);
	}
}
