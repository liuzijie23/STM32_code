#ifndef _key_H
#define _key_H
#include <stm32f10x.h>

#define res GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)

void Key_IO_Init(void);
void key_pros(void);

#endif
