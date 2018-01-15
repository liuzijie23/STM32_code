#ifndef _dht11_H
#define _dht11_H
#include <stm32f10x.h>

#define BOOL unsigned char

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

void DHT11_DataPin_Configure_Output(void);
void DHT11_DataPin_Configure_Input(void);
BOOL DHT11_get_databit(void);
void DHT11_set_databit(BOOL level);
void mdelay(u16 ms);
void udelay(u16 us);
uint8_t DHT11_read_byte(void);
uint8_t DHT11_start_sampling(void);
void DHT11_get_data(void);
#endif
