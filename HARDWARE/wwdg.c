/****************************************************************************************************
@author: TuRan
@data: 
@des: 	窗口看门狗相关代码
****************************************************************************************************/
#include "wwdg.h"
#include "led.h"

u8 WWDG_CNT = 0x7f;

/**
 * @brief 初始化窗口看门狗
 * @param tr (u8) 计数器初值
 * @param wr (u8) 窗口值
 * @param fprer (u32) 预分频系数
 */
void WWDG_Init(u8 tr, u8 wr, u32 fprer)
{
	// 配置窗口看门狗
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	// 使能窗口看门狗时钟
	WWDG_CNT = tr & WWDG_CNT;								// 将 tr 的值与 WWDG_CNT相与, 取格式化之后的值
	WWDG_SetPrescaler(fprer);								// 设置预分频系数
	WWDG_SetWindowValue(wr);								// 设置上窗口值
	WWDG_Enable(WWDG_CNT);									// 使能窗口看门狗, 并且设置窗口看门狗当前计数器的值

	// 配置窗口看门狗EWI中断
	WWDG_ClearFlag();										// 清楚窗口标志位, (此为必需由软件置0)
	WWDG_NVICInit();										// 初始化NVIC
	WWDG_EnableIT();										// 初始化WWDG中断: 设置CFR寄存器的第9位(EWI) 为 1 
	
}

/**
 * @brief 初始化WWDG的NVIC中断优先级管理 
 */
void WWDG_NVICInit()
{
	NVIC_InitTypeDef NI;
	NI.NVIC_IRQChannel = WWDG_IRQn;
	NI.NVIC_IRQChannelPreemptionPriority = 2; 
	NI.NVIC_IRQChannelSubPriority = 3;
	NI.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NI);
}




