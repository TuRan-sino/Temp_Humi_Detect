#ifndef __TIMER_H__
#define __TIMER_H__
#include "sys.h"


void Time_MyInit(TIM_TypeDef* TIMx, u16 arr, u16 psc);
void PWM_MyInit(u16 arr, u16 psc);
void PWM_Flicker(void);
void Cap_MyInit(u16 arr,u16 psc);
void CAP_KEY();


#endif
