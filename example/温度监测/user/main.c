#include <stm32f10x.h>
#include "key.h"
#include "adc.h"
#include "usart.h"
#include "delay.h"
#include "printf.h"
#include "pwm.h"
#include "oled.h"
#include "ds18b20.h"

u16 k = 0;
extern u16 temperature[2];
extern uint8_t temp1,temp2;
int main()
{
	//adc_init();
	usart_init();
	printf_init();
	Key_IO_Init();	// PC0、PC1、PC2、PC3
	OLED_Init();			//初始化OLED  
	OLED_Clear();
	DS18B20_Init();
	DS18B20_GPIO_Config();
	printf("...start...\r\n");
	pwm_init();
	
	OLED_ShowCHinese(0,0,0);	//当
	OLED_ShowCHinese(16,0,1);	//前
	OLED_ShowCHinese(32,0,2);	//温
	OLED_ShowCHinese(48,0,3);	//度
	OLED_ShowCHinese(0,2,4);	//设
	OLED_ShowCHinese(16,2,5);	//定
	OLED_ShowCHinese(32,2,6);	//范
	OLED_ShowCHinese(48,2,7);	//围
	OLED_ShowString(72,2,"max",8);
	OLED_ShowString(72,3,"min",8);
	OLED_ShowNum(96,2,temperature[0],4,8);
	OLED_ShowNum(96,3,temperature[1],4,8);
	while((temp1 + 0.1*temp2) > temperature[0] || (temp1 + 0.1*temp2) < temperature[1])
	{
		delay_ms(100);
		DS18B20_GetTemp();
	}
	
	while(1)
	{
		key_pros();
		// 判断是否需要报警
		if ((temp1 + 0.1*temp2) > temperature[0] || (temp1 + 0.1*temp2) < temperature[1])
			TIM_SetCompare2(TIM4, 9000);
		else TIM_SetCompare2(TIM4, 0);
		
		delay_ms(50);
		k++;
		if  (k == 8)
		{
			k = 0;
			DS18B20_GetTemp();
			printf("temp = %d.%d\r\n",temp1,temp2);
			OLED_ShowNum(72,0,temp1,3,16);
			OLED_ShowNum(96,0,temp2,2,16);
			OLED_ShowChar(96,0,'.',16);
		}
	}
}
