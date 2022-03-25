#ifndef __WWDG_H__
#define __WWDG_H__
#include "sys.h"


// 格式化计数器的值, 0x7f = 011111111
extern u8 WWDG_CNT;

// void WWDG_MyInit(u8 tr, u8 wr, u32 fprer);
// void WWDG_NVICInit(void);


void WWDG_MyInit(u8 tr,u8 wr,u32 fprer);
void WWDG_NVICInit(void);

#endif
