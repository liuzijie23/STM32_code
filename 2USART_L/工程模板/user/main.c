#include <stm32f10x.h>
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "public.h"

u16 i=0;

int main()
{
	usart_init();
	Key_IO_Init();
	GPIO_Write(GPIOA,(u16)0x00);
	while(1)
	{
		key_pros();
		delay_us(2000);
	}
}
