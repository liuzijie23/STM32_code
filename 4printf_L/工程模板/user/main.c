#include <stm32f10x.h>
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "printf.h"
#include "timer.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"

u16 i=0;

int main()
{
	timer_init();
	usart_init();
	printf_init();
	Key_IO_Init();
	delay_us(2000);
	printf("kuangda\r\n");
	printf("kuangda\r\n");
	GPIO_Write(GPIOA,(u16)0x00);
	while(1)
	{
		key_pros();
		delay_us(2000);
	}
}
