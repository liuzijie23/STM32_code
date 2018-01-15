#ifndef _key_H
#define _key_H
#include <stm32f10x.h>

#define res1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)
#define res2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)
#define res3 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)
#define res4 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)

void Key_IO_Init(void);
void key_pros(void);

#endif
