#ifndef __DHT_H__
#define __DHT_H__
#include "sys.h"

// 上拉/下拉输入模式
#define DHT11_IO_IN()             \
    {                             \
        GPIOG->CRL &= 0XFFFFFFF0; \
        GPIOG->CRL |= 8;          \
    }
// 通用推完输出模式, 最大速度为50mhz
#define DHT11_IO_OUT()            \
    {                             \
        GPIOG->CRL &= 0XFFFFFFF0; \
        GPIOG->CRL |= 3;          \
    }


#define DHT11_DQ_OUT PGout(0)
#define DHT11_DQ_IN PGin(0)

u8 DHT11_Init(void); 
u8 DHT11_Read_Data(u8* temp, u8* humi); 
u8 DHT11_Read_Byte(void); 
u8 DHT11_Read_Bit(void);
u8 DHT11_Check(void); 
void DHT11_Rst(void);
#endif