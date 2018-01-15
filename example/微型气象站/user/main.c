#include <stm32f10x.h>
#include "key.h"
#include "adc.h"
#include "usart.h"
#include "delay.h"
#include "printf.h"
#include "pwm.h"
#include "oled.h"
#include "dht11.h"

u16 i = 0,j = 0,k = 0;
extern u16 temperature[2],himidity[2],L_intensity[2];
int main()
{
	adc_init();
	usart_init();
	printf_init();
	Key_IO_Init();
	OLED_Init();			//初始化OLED  
	OLED_Clear();
	printf("...start...\r\n");
	
	//OLED_ShowString(0,0,"01234LEDTEST",8);
	
	OLED_ShowCHinese(0,0,0);//
	OLED_ShowCHinese(16,0,1);//
	OLED_ShowCHinese(64,0,2);//
	OLED_ShowCHinese(80,0,3);//
	OLED_ShowCHinese(0,2,4);//
	OLED_ShowCHinese(16,2,5);//
	//OLED_ShowCHinese(64,2,6);//
	//OLED_ShowCHinese(80,2,7);//
	OLED_ShowString(80,2,"max",8);
	OLED_ShowString(80,3,"min",8);
	//TIM_SetCompare1(TIM4,0);
	while(1)
	{
		key_pros();
		delay_us(2000);
		k++;
		if  (k == 80)
		{	
			k = 0;
			j++;
			if (j == 10)
			{	
				j = 0;
				DHT11_get_data();
			}
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//转换结束标志位
			printf("ADC = %d\r\n",adc_result());
			OLED_ShowNum(35,2,adc_result(),3,16);
			if (adc_result()<L_intensity[0]) GPIO_SetBits(GPIOA,GPIO_Pin_6); else GPIO_ResetBits(GPIOA,GPIO_Pin_6);
			if (adc_result()>L_intensity[1]) GPIO_SetBits(GPIOA,GPIO_Pin_7); else GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		}
	}
}
