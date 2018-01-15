#include "dht11.h"
#include "delay.h"
#include "oled.h"
#include <stm32f10x.h>
#include "printf.h"
extern u16 temperature[2],himidity[2],L_intensity[2];

u8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
u8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;

void DHT11_DataPin_Configure_Output(void)
{
     GPIO_InitTypeDef  GPIO_InitStructure;
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
                
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void DHT11_DataPin_Configure_Input(void)
{
	GPIO_InitTypeDef  DataPin;
	DataPin.GPIO_Pin = GPIO_Pin_12;             
	DataPin.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	DataPin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &DataPin);
}

BOOL DHT11_get_databit(void)
{
	uint8_t val;

	val = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
	if(val == Bit_RESET){
	return FALSE;
	}else{
	return TRUE;
	}
}

void DHT11_set_databit(BOOL level)
{
	if(level == TRUE){
		GPIO_SetBits(GPIOC, GPIO_Pin_12);
	}else{
		GPIO_ResetBits(GPIOC, GPIO_Pin_12);
	}
}

void mdelay(u16 ms)  
{
    if(ms != 0){
        delay_us(ms*1000);
    }
}

void udelay(u16 us)  
{
    if(us != 0){
        delay_us(us);
    }
}
uint8_t DHT11_read_byte(void)
{     
    uint8_t i;
    uint8_t data = 0;

    for(i = 0; i < 8; i++)
    {
        data <<= 1;
           while((!DHT11_get_databit()));
        udelay(10);
        udelay(10);
        udelay(10);

        if(DHT11_get_databit())    {
            data |= 0x1;
            while(DHT11_get_databit());
        } else{
        
        }
     }
                         
     return data;
}
  
uint8_t DHT11_start_sampling(void)
{
    DHT11_DataPin_Configure_Output();
    DHT11_set_databit(FALSE);
    mdelay(18);
    DHT11_set_databit(TRUE);
    udelay(20);
    DHT11_set_databit(TRUE);

    DHT11_DataPin_Configure_Input();    
    if(!DHT11_get_databit())
    {
       while((!DHT11_get_databit()));
       while((DHT11_get_databit()));
       return 1;
    }

    return 0;               
}

void DHT11_get_data(void)
{      
    u8 temp;
    if(DHT11_start_sampling())
	{
		U8RH_data_H_temp = DHT11_read_byte();
		U8RH_data_L_temp = DHT11_read_byte();
		U8T_data_H_temp  = DHT11_read_byte();
		U8T_data_L_temp  = DHT11_read_byte();
		U8checkdata_temp = DHT11_read_byte();

		DHT11_DataPin_Configure_Output();   
		DHT11_set_databit(TRUE);

		temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
		
		if(temp==U8checkdata_temp)
		{
			U8RH_data_H=U8RH_data_H_temp;
			U8RH_data_L=U8RH_data_L_temp;
			U8T_data_H=U8T_data_H_temp;
			U8T_data_L=U8T_data_L_temp;
			U8checkdata=U8checkdata_temp;
			printf("temp = %d\r\n",U8T_data_H);
			printf("humi = %d\r\n",U8RH_data_H);
			OLED_ShowNum(35,0,U8T_data_H,3,16);
			OLED_ShowNum(99,0,U8RH_data_H,3,16);
			if (U8T_data_H<temperature[0]) GPIO_SetBits(GPIOA,GPIO_Pin_2); else GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			if (U8T_data_H>temperature[1]) GPIO_SetBits(GPIOA,GPIO_Pin_3); else GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			if (U8RH_data_H<himidity[0]) GPIO_SetBits(GPIOA,GPIO_Pin_4); else GPIO_ResetBits(GPIOA,GPIO_Pin_4);
			if (U8RH_data_H>himidity[1]) GPIO_SetBits(GPIOA,GPIO_Pin_5); else GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		}
		else printf("checksum failure \r\n");
	}
		else{
        printf("DHT11 didn't answer. Please try again.\r\n");
    }
}
