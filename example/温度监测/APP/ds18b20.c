#include "ds18b20.h"
#include "delay.h"

#define RCC_DS18B20  RCC_APB2Periph_GPIOB
#define GPIO_DS18B20_PORT GPIOB
#define GPIO_DS18B20_Pin GPIO_Pin_6

#define DS18B20_H GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define DS18B20_L GPIO_ResetBits(GPIOB,GPIO_Pin_6)

uint8_t temp1,temp2;   /* 温度传感器数值：整数部分与小数部分 */

/*
* 函数名：DS18B20_GPIO_Config
* 描述  ：配置Relay用到的I/O口
* 输入  ：无
* 输出  ：无
*/
void DS18B20_GPIO_Config(void)
{                
	GPIO_InitTypeDef GPIO_InitStructure;        

	/*开启DS18B20对应的GPIO 的外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_DS18B20, ENABLE);         
	/*选择要控制的DS18B20引脚*/                                                                                                                           
	GPIO_InitStructure.GPIO_Pin = GPIO_DS18B20_Pin;        
	/*设置引脚模式为 OD输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化相应GPIO*/
	GPIO_Init(GPIO_DS18B20_PORT, &GPIO_InitStructure);        
}


void DS18B20_Reset(void)
{
        DS18B20_H;
        delay_us(20);                 /*  10us延时  */
        DS18B20_L;
        delay_us(550);    /*  550us延时  */
        DS18B20_H;
        while(GPIO_ReadInputDataBit(GPIO_DS18B20_PORT,GPIO_DS18B20_Pin));
        delay_us(500);                /*  500us延时  */
        DS18B20_H;
}


void DS18B20_Init(void)
{
        DS18B20_Reset();
        DS18B20_WriteByte(0xCC);
        DS18B20_WriteByte(0x4E);
        DS18B20_WriteByte(0x64);
        DS18B20_WriteByte(0x8A);
        DS18B20_WriteByte(0x1F);
}


void DS18B20_WriteBit0(void)
{
        DS18B20_H;
        delay_us(1);                /*  1us延时  */
        DS18B20_L;
        delay_us(55);   /*  55us延时  */
        DS18B20_H;
        delay_us(1);    /*  1us延时  */
}

void DS18B20_WriteBit1(void)
{
        DS18B20_H;
        delay_us(1);    /*  1us延时  */
        DS18B20_L;
        delay_us(5);           /*  5us延时  */
        DS18B20_H;
        delay_us(5);           /*  5us延时  */
        delay_us(50);   /*  50us延时  */
}

uint8_t DS18B20_ReadBit(void)
{
        uint8_t bdata;
        DS18B20_H;
        delay_us(1);           /*  1us延时  */
        DS18B20_L;
        delay_us(4);           /*  4us延时  */
        DS18B20_H;
        delay_us(8);           /*  8us延时  */        
        bdata=GPIO_ReadInputDataBit(GPIO_DS18B20_PORT,GPIO_DS18B20_Pin);
        delay_us(60);   /*  60us延时  */
        delay_us(2);    /*  2us延时  */
        return bdata;
}

void DS18B20_WriteByte(uint8_t udata)
{
        uint8_t i;
        for(i=0;i<8;i++)
        {
                if(udata&0x01)
                        DS18B20_WriteBit1();
                else
                        DS18B20_WriteBit0();
                udata=udata>>1;
        }
        delay_us(10);  /*  10us延时  */
}

uint8_t DS18B20_ReadByte(void)
{
        uint8_t i,udata,j;
        udata=0;
        for(i=0;i<8;i++)
        {
                udata=udata>>1;
                j=DS18B20_ReadBit();
                if(j==0x01)
                        udata|=0x80;
                else
                        udata|=0x00;
                delay_us(2);        /*  2us延时  */
        }
        return udata;
}

void DS18B20_GetTemp(void)
{
        DS18B20_WriteByte(0xCC);
        DS18B20_WriteByte(0x44);
        delay_ms(100);
        DS18B20_Reset();
        DS18B20_WriteByte(0xCC);
        DS18B20_WriteByte(0xBE);
        
        temp2=DS18B20_ReadByte();
        temp1=DS18B20_ReadByte();
        DS18B20_ReadByte();
        DS18B20_ReadByte();
        DS18B20_ReadByte();
        DS18B20_ReadByte();
        DS18B20_ReadByte();
        DS18B20_ReadByte();
        DS18B20_ReadByte();
        
        temp1=temp1<<4;
        temp1+=temp2>>4;
		temp2=(temp2&0x0F)/1.6;
        //temp2=(temp2&0x0F)?5:0;
        DS18B20_Reset();        
}
