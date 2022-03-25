/**
 * @author TuRan
 * @def usart串口相关函数. 
 * @attention 我不知道为什么, 每一次重新烧录之后使用串口都会发生接收乱码的形况. 为了解决这个问题只有重新烧录之后重新拔插CH340  
*/
#ifndef _USART_H_
#define _USART_H_

#include "stdio.h"
#include "sys.h"

#define USART_REC_LEN 200	// 定义最大接收字符数
#define EN_USART1_RX 1 		// 使能串口1接收

// 外部变量, 接收缓冲
// 接收且暂存来自外部的数据
extern u8 USART_RX_BUF[USART_REC_LEN]	;

// USART Recive Status --> 状态标记位
// bit 15 | bit14 | bit13~0
// 接收完成标记 (接收到0X0D标志的同时接收到0X0A标志)	| 接收到0x0D标志 (CR) | 接收到有效数据个数	
extern u16 USART_RX_STA;


void Usart_MyInit(u16 serial, u32 buterate);
#endif
