#include <stm32f10x.h>
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "adc.h"
//#include "public.h"
#include "timer.h"
#include "printf.h"

#define N 10
#define M 2

vu16 AD_Value[N][M];
//vu16 After_filter[M];
u16 i=0;
int main()
{
	usart_init();
	printf_init();
	adc_init();
	Key_IO_Init();
	timer_init();
//	GPIO_Write(GPIOA,(u16)0x00);
	printf("...¿ªÊ¼...");
	while(1)
	{
		key_pros();
		delay_us(2000);
	}
}
