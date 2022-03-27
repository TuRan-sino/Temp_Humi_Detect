/****************************************************************************************************
@author: TuRan
@data: 
@des: 	通用定时器文件
		包含了通用定时器, 和PWM单元相关代码
****************************************************************************************************/
#include "timer.h"
#include "delay.h"


/**
 * @brief 通用定时器初始化函数 
 * @param timx 表示当前初始化的时那个通用定时器
 * @param arr 自动重装载寄存器初值
 * @param psc 预分频器初值
 */
void TIM_Init_Timer3(TIM_TypeDef* TIMx, u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIT_Timer;
	NVIC_InitTypeDef NIT_Timer;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		// 使能TIM3时钟
	
	NIT_Timer.NVIC_IRQChannel = TIM3_IRQn;						// 设置终端通道
	NIT_Timer.NVIC_IRQChannelCmd = ENABLE;						// 使能
	NIT_Timer.NVIC_IRQChannelPreemptionPriority = 0; 			// 设置抢占优先级
	NIT_Timer.NVIC_IRQChannelSubPriority = 3;					// 设置响应优先级
	NVIC_Init(&NIT_Timer);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	TIT_Timer.TIM_ClockDivision = TIM_CKD_DIV1;					// 设置时钟分屏
	TIT_Timer.TIM_CounterMode = TIM_CounterMode_Up;				// 设置计数模式
	TIT_Timer.TIM_Period = arr;									// 设置arr寄存器(arr寄存器是用来设定定时其最大值的寄存器)
	TIT_Timer.TIM_Prescaler = psc;								// 设置psc寄存器(psc寄存器用来决定分频系数)
	TIM_TimeBaseInit(TIMx, &TIT_Timer);
	TIM_Cmd(TIMx, ENABLE);										// 使能TIM3
}