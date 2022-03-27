/**
 * @author TuRan
 * @def usart串口相关函数. 
 * @attention 我不知道为什么, 每一次重新烧录之后使用串口都会发生接收乱码的形况. 为了解决这个问题只有重新烧录之后重新拔插CH340  
*/
#ifndef _USART_H_
#define _USART_H_

#include "stdio.h"
#include "sys.h"

#define USART_MAX_LEN 10	// 定义最大接收字符数



void Usart_Init_USART1(u32 buterate);
void Usart_Init_USART3(u32 buterate);
void Usart_SendString(char *s);
#endif
