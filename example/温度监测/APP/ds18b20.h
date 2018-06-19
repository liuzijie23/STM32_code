#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f10x.h"

void DS18B20_GPIO_Config(void);
void DS18B20_Reset(void);
void DS18B20_Init(void);
void DS18B20_WriteBit1(void);
void DS18B20_WriteBit0(void);
uint8_t DS18B20_ReadBit(void);
void DS18B20_WriteByte(uint8_t udata);
uint8_t DS18B20_ReadByte(void);
void DS18B20_GetTemp(void);
#endif /* __LED_H */
