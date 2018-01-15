#include "key.h"
#include "delay.h"
#include "oled.h"
#include "printf.h"
#include <stm32f10x.h>                  // Device header

extern u16 i;
int w = 3;
u16 temperature[2] = {20,50},himidity[2] = {20,80},L_intensity[2] = {50,500};
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
			if (i == 0)
			{ 
				OLED_ShowCHinese(64,2,0);//温度
				OLED_ShowNum(108,3,temperature[0],3,12);
				OLED_ShowNum(108,2,temperature[1],3,12);
			}
			else if (i == 1)
			{ 
				OLED_ShowCHinese(64,2,2);//湿度
				OLED_ShowNum(108,3,himidity[0],3,12);
				OLED_ShowNum(108,2,himidity[1],3,12);	
			}
			else 
			{
				OLED_ShowCHinese(64,2,4);//光强
				OLED_ShowNum(108,3,L_intensity[0],3,12);
				OLED_ShowNum(108,2,L_intensity[1],3,12);
			}
			printf("... s1 pushed ...\r\n");
			i++;
			if (i==3) i = 0;
		}
		while(!res1);
	}
	if(res2==0)
	{
		delay_us(1000);
		if(res2==0)
		{
			switch (i+w)
			{
				case 1:{OLED_ShowNum(108,3,++temperature[0],3,12);}break;
				case 2:{himidity[0] += 2;OLED_ShowNum(108,3,himidity[0],3,12);}break;
				case 0:{L_intensity[0] += 20;OLED_ShowNum(108,3,L_intensity[0],3,12);}break;
				case 4:{OLED_ShowNum(108,3,--temperature[0],3,12);}break;
				case 5:{himidity[0] -= 2;OLED_ShowNum(108,3,himidity[0],3,12);}break;
				case 3:{L_intensity[0] -= 20;OLED_ShowNum(108,3,L_intensity[0],3,12);}break;
				default : break;
			}

			printf("... s2 pushed ...\r\n");
		}
		while(!res2);
	}
	if(res3==0)
	{
		delay_us(1000);
		if(res3==0)
		{
			switch (i+w)
			{
				case 4:{OLED_ShowNum(108,2,++temperature[1],3,12);}break;
				case 5:{himidity[1] += 2;OLED_ShowNum(108,2,himidity[1],3,12);}break;
				case 3:{L_intensity[1] += 20;OLED_ShowNum(108,2,L_intensity[1],3,12);}break;
				case 1:{OLED_ShowNum(108,2,--temperature[1],3,12);}break;
				case 2:{himidity[1] -= 2;OLED_ShowNum(108,2,himidity[1],3,12);}break;
				case 0:{L_intensity[1] -= 20;OLED_ShowNum(108,2,L_intensity[1],3,12);}break;
				default : break;
			}
			printf("... s3 pushed ...\r\n");
		}
		while(!res3);
	}
	if(res4==0)
	{
		delay_us(1000);
		if(res4==0)
		{
			if (w == 0) w = 3;
			else w = 0;
			printf("... s4 pushed ...\r\n");
		}
		while(!res4);
	}

}
