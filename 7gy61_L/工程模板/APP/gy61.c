#include "gy61.h"
#include "stm32f10x.h"

extern float Du_x,Du_y;//定义x方向偏角、y方向偏角

float N2D_x(uint16_t N_x)
{
	return ((N_x-2040.5)*0.087890625);
}

float N2D_y(uint16_t N_y)
{
	return ((N_y - 2046.4)*0.087890625);
}

