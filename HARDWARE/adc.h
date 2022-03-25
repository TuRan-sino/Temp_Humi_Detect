#ifndef __ADC_H__
#define __ADC_H__
#include "sys.h"

void ADC_MyInit(void);
u16 GET_ADC(u8 ch);
u16 Get_ADC_Average(u8 ch, u8 times);

#endif