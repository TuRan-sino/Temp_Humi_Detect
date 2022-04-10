#ifndef __SGP30_H__
#define __SGP30_H__
#include "sys.h"


#define Read_Addr 0xb1
#define Write_Addr 0xb0

u32 SGP30_Read(void);
void SGP30_Write(u8 a, u8 b);
int SGP30_Init(void);
void SGP30_Read_Data(u16 *CO2, u16 *TVOC);

#endif