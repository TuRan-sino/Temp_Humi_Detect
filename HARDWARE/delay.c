#include "delay.h"


static u8 fac_us = 0;
static u16 fac_ms = 0;


/**
 * @brief 初始化延迟函数
 * @retval void
*/
void delay_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);			// 设置系统时钟为 HCLK 的 8分屏
	fac_us = SystemCoreClock/8000000;								// 设置纳秒为9 (72000000 / 8000000)
	fac_ms = (u16)fac_us * 1000;									// 设置毫秒分频为 9 * 1000 = 9000
}


/**
 * @brief 微秒延迟函数
 * @param nums: u32 需要延迟的毫秒
 * @retval void
*/
void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD = nus * fac_us;					// 将时间装载到LOAD寄存器上
	SysTick->VAL = 0x00;							// 清空计数器	
	// 开始倒数
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;		// 开启计数器
	do{
		temp = SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (1 << 16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		// 关闭计数器
	SysTick->VAL = 0x00;							// 计数器清空
}


/**
 * @brief 毫秒延迟函数
 * @param nums: u16 需要延迟的微秒
 * @retval void
*/
void delay_ms(u16 nums)
{
	u32 temp;
	SysTick->LOAD = (u32)nums * fac_ms;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do{
		temp = SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0x00;
}