#ifndef __DHT11_H
#define __DHT11_H
#include "sys.h"


#define DHT11_OUT PGout(0)
#define DHT11_IN PGin(0)


void DHT11_Set_INPUT(void);
void DHT11_Set_OUTPUT(void);
u8 DHT11_Set_Respond(void);
void DHT11_Set_Start(void);
void Dht11_Init(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Data(u8 *temp, u8 *humi);

#endif
