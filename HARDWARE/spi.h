#ifndef __SPI_H__
#define __SPI_H__
#include "sys.h"
#include "stdbool.h"


void SPI_Init_SPI2();
u8 SPI_Transmit(u8 data);
u8 SPI_Write(u8 *data, u16 length);

#endif