#ifndef __SPI_H__
#define __SPI_H__
#include "sys.h"
#include "stdbool.h"


void Spi_Init_Spi2();
u8 Spi2_Transmit(u8 data);
u8 Spi_Write(u8 *data, u16 length);

#endif