/****************************************************************************************************
@author: TuRan
@des: 
	KEY_UP 连接 PA0
	KEY_1 连接 PE3
	KEY_0 连接 PE4
****************************************************************************************************/

#ifndef _KEY_H_
#define _KEY_H_

#include "sys.h"
#include "delay.h"

// 定义端口名称
#define KEY1 PEin(3)
#define KEY0 PEin(4)
#define KEY_UP PAin(0)

// 定义返回值名称
#define KEY0_PRESS 1
#define KEY1_PRESS 2
#define KEY_UP_PRESS 3

// 定义KEY按下和松开
#define KEY_ON 0
#define KEY_OFF 1

u8 KEY_Scan(u8 mode);
void KEY_Init(void);


#endif
