#ifndef _DELAY_H_
#define _DELAY_H_
#include "sys.h"


void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

void Delay_MyInit(void);
void Delay_Myus(u32 nus);
void Delay_Myms(u16 nms);

#endif
