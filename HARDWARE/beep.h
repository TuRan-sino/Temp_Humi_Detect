#ifndef _BEEP_H_
#define _BEEP_H_
#include "sys.h"
#include "delay.h"

// 宏定义BEEP端口
// 1 --> 响		0 --> 关闭
#define BEEP PBout(8)
#define BEEP_ON 1
#define BEEP_OFF 0

void BEEP_Init(void);
void BEEP_Alarm(u32 temp, u32 humi);


#endif
