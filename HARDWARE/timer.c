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
void TIM_Init_Timer3(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIT_Timer;
	NVIC_InitTypeDef NIT_Timer;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		// 使能TIM3时钟
	
	NIT_Timer.NVIC_IRQChannel = TIM3_IRQn;						// 设置终端通道
	NIT_Timer.NVIC_IRQChannelCmd = ENABLE;						// 使能
	NIT_Timer.NVIC_IRQChannelPreemptionPriority = 0; 			// 设置抢占优先级
	NIT_Timer.NVIC_IRQChannelSubPriority = 3;					// 设置响应优先级
	NVIC_Init(&NIT_Timer);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIT_Timer.TIM_ClockDivision = TIM_CKD_DIV1;					// 设置时钟分屏
	TIT_Timer.TIM_CounterMode = TIM_CounterMode_Up;				// 设置计数模式
	TIT_Timer.TIM_Period = arr;									// 设置arr寄存器(arr寄存器是用来设定定时其最大值的寄存器)
	TIT_Timer.TIM_Prescaler = psc;								// 设置psc寄存器(psc寄存器用来决定分频系数)
	TIM_TimeBaseInit(TIM3, &TIT_Timer);
	TIM_Cmd(TIM3, ENABLE);										// 使能TIM3
}



/**
 * @brief 定时器5通道2输入捕获配置
 * @param 
 * @param 
 * @retval 
 * @note 
*/
void TIM_Init_Timer5(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM5_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //使能TIM5时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PA端口时钟

    //设置GPIOA.1为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // PA1 端口配置
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure); //设置为浮空输入

    //初始化TIM5
    TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数模式
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
                                                    //初始化通道2
    TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; // CC1S=01 	选择输入端 IC2映射到TI5上
    TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿捕获
    TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //配置输入分频,不分频
    TIM5_ICInitStructure.TIM_ICFilter = 0x03; // IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
    TIM_ICInit(TIM5, &TIM5_ICInitStructure); //初始化I5 IC2

    TIM_Cmd(TIM5, ENABLE); //使能定时器5
}