#ifndef _pid_H
#define _pid_H
#include "stm32f10x.h"

void PIDM1_Init(void);
int32_t PIDM1_PosLocCalc(float NextPoint);

#endif
