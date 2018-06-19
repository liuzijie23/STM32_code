#include "ds18b20.h"
#include "delay.h"

#define RCC_DS18B20  RCC_APB2Periph_GPIOB
#define GPIO_DS18B20_PORT GPIOB
#define GPIO_DS18B20_Pin GPIO_Pin_6

#define DS18B20_H GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define DS18B20_L GPIO_ResetBits(GPIOB,GPIO_Pin_6)

uint8_t temp1,temp2;   /* �¶ȴ�������ֵ������������С������ */

/*
* ��������DS18B20_GPIO_Config
* ����  ������Relay�õ���I/O��
* ����  ����
* ���  ����
*/
void DS18B20_GPIO_Config(void)
{                
	GPIO_InitTypeDef GPIO_InitStructure;        

	/*����DS18B20��Ӧ��GPIO ������ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_DS18B20, ENABLE);         
	/*ѡ��Ҫ���Ƶ�DS18B20����*/                                                                                                                           
	GPIO_InitStructure.GPIO_Pin = GPIO_DS18B20_Pin;        
	/*��������ģʽΪ OD���*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ����ӦGPIO*/
	GPIO_Init(GPIO_DS18B20_PORT, &GPIO_InitStructure);        
}


void DS18B20_Reset(void)
{
        DS18B20_H;
        delay_us(20);                 /*  10us��ʱ  */
        DS18B20_L;
        delay_us(550);    /*  550us��ʱ  */
        DS18B20_H;
        while(GPIO_ReadInputDataBit(GPIO_DS18B20_PORT,GPIO_DS18B20_Pin));
        delay_us(500);                /*  500us��ʱ  */
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
        delay_us(1);                /*  1us��ʱ  */
        DS18B20_L;
        delay_us(55);   /*  55us��ʱ  */
        DS18B20_H;
        delay_us(1);    /*  1us��ʱ  */
}

void DS18B20_WriteBit1(void)
{
        DS18B20_H;
        delay_us(1);    /*  1us��ʱ  */
        DS18B20_L;
        delay_us(5);           /*  5us��ʱ  */
        DS18B20_H;
        delay_us(5);           /*  5us��ʱ  */
        delay_us(50);   /*  50us��ʱ  */
}

uint8_t DS18B20_ReadBit(void)
{
        uint8_t bdata;
        DS18B20_H;
        delay_us(1);           /*  1us��ʱ  */
        DS18B20_L;
        delay_us(4);           /*  4us��ʱ  */
        DS18B20_H;
        delay_us(8);           /*  8us��ʱ  */        
        bdata=GPIO_ReadInputDataBit(GPIO_DS18B20_PORT,GPIO_DS18B20_Pin);
        delay_us(60);   /*  60us��ʱ  */
        delay_us(2);    /*  2us��ʱ  */
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
        delay_us(10);  /*  10us��ʱ  */
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
                delay_us(2);        /*  2us��ʱ  */
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
