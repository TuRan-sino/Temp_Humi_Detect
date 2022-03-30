#ifndef __DHT11_H
#define __DHT11_H
#include "sys.h"

// 上拉/下拉输入模式
#define DHT11_MODE_IN()             \
    {                             \
        GPIOG->CRL &= 0XFFFFFFF0; \
        GPIOG->CRL |= 8;          \
    }
// 通用推完输出模式, 最大速度为50mhz
#define DHT11_MODE_OUT()            \
    {                             \
        GPIOG->CRL &= 0XFFFFFFF0; \
        GPIOG->CRL |= 3;          \
    }




#define DHT11_PIN_OUT PGout(0)
#define DHT11_DQ_IN PGin(0)

#define DHT11_OUT PGout(0)
#define DHT11_IN PGin(0)

u8 DHT11_Init(void); 
u8 DHT11_Read_Data(u8* temp, u8* humi); 
u8 DHT11_Read_Byte(void); 
u8 DHT11_Read_Bit(void);
u8 DHT11_Check(void); 
void DHT11_Rst(void);


void DHT11_Set_INPUT(void);
u8 DHT11_Wait_Respond(void);
void DHT11_Set_OUTPUT(void);
void DHT11_SendStart(void);
void DHT11_Init_Mine(void);
u8 DHT11_Read_Bit_Mine(void);
u8 DHT11_Read_Byte_Mine(void);
u8 DHT11_Read_Data_Mine(u8 *temp, u8 *humi);

#endif
