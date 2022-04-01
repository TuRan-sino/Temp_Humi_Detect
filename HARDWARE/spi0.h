#ifndef __SPI0_H__
#define __SPI0_H__
#include "sys.h"
#include "stdbool.h"


void Spi_Init_Spi2();
u8 Spi2_Transmit(u8 data);
bool Spi2_SetSpeed(u8 Prescaler);

#endif