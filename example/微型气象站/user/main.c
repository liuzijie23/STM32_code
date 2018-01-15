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
	OLED_ShowCHinese(64,2,6);//
	OLED_ShowCHinese(80,2,7);//
	
	//TIM_SetCompare1(TIM4,0);
	while(1)
	{
		key_pros();
		delay_us(2000);
		k++;
		if  (k == 100)
		{	
			k = 0;
			j++;
			if (j == 8)
			{	
				j = 0;
				DHT11_get_data();				
			}
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//转换结束标志位
			printf("ADC = %d\r\n",adc_result());
			OLED_ShowNum(35,2,adc_result(),3,16);			
		}
	}
}
