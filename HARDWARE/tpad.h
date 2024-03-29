#ifndef __TPAD_H__
#define __TPAD_H__
#include "sys.h"

extern vu16 tpad_default_val;

void TPAD_Reset(void);
u16 TPAD_Get_Val(void);
u16 TPAD_Get_MaxVal(u8 n);
u8 TPAD_Init(u8 psc);
u8 TPAD_Scan(u8 mode);

#endif
