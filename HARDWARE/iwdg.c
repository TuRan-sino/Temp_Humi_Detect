/****************************************************************************************************
@author: TuRan
@data: 
@des: 	独立看门狗相关代码
****************************************************************************************************/

#include "iwdg.h"

/**
 * @brief 看门狗初始化函数 
 * @param prer 预分频系数
 * @param rlr 重装载计数器初值
 */
void IWDG_Init(u8 prer, u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		// 使能取消写保护
	IWDG_SetPrescaler(prer);							// 设置预分频系数
	IWDG_SetReload(rlr);								// 设置重装载值

	IWDG_ReloadCounter();								// 开始喂狗(将看门狗的KR寄存器启动的时候的数值设置为RLR寄存器的数值)
	IWDG_Enable();										// 使能IWDG
}
