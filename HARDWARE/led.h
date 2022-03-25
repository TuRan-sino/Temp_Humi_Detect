/****************************************************************************************************
@author: TuRan
@des:
	LED0 连接的 PB5
	LED1 连接的 PE5
****************************************************************************************************/
#ifndef _LED_H_
#define _LED_H_
#include "sys.h"

// 宏定义端口
// 为0时点亮		为1时关闭
#define LED0 PBout(5)
#define LED1 PEout(5)
#define LED_ON 0
#define LED_OFF 1


void LED_Init(void);


#endif