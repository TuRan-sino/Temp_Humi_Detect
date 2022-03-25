/****************************************************************************************************
@author: TuRan
@data: 
@des: 	通用定时器文件
		包含了通用定时器, 和PWM单元相关代码
****************************************************************************************************/
#include "timer.h"
#include "delay.h"
#include <stdio.h>

/* 
捕获状态寄存器
|bit7			|bit6				|bit5-0|
|捕获完成标志	|捕获到高电平标志	|捕获到高电平之后溢出的次数|
*/
u8  CAPTURE_STA = 0;

// 用于记录捕获到下降沿的时候 TIM_CNT(CNT计数器)的值
u16	CAPTURE_VAL;


/**
 * @brief 通用定时器初始化函数 
 * @param timx 表示当前初始化的时那个通用定时器
 * @param arr 自动重装载寄存器初值
 * @param psc 预分频器初值
 */
void Time_MyInit(TIM_TypeDef* TIMx, u16 arr, u16 psc)
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

/**
 * @brief 初始化PWM脉冲发射器, 输出到LED0上
 * @param arr [u16] 定时器周期
 * @param psc [u16] 定时器分频系数
*/
void PWM_MyInit(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TTBIT;
	GPIO_InitTypeDef GIT;
	TIM_OCInitTypeDef TOIT;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		// 初始化GPIOB的始终
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		// 初始化TIM3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		// 由于LED0与TIM3的通道2输出口相同, 因此需要开启端口复用时钟

	GIT.GPIO_Mode = GPIO_Mode_AF_PP;							// 复用推挽
	GIT.GPIO_Pin = GPIO_Pin_5;									// GPIOB.5
	GIT.GPIO_Speed = GPIO_Speed_50MHz;							// 输出速度
	GPIO_Init(GPIOB, &GIT);

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);		// 这里除了复用还是用到了重影响, 本来tim3的通道2是印射在PB4上面, 使用重映像映射到PB5

	TTBIT.TIM_ClockDivision = TIM_CKD_DIV1;						// 配置定时器时钟与数字滤波器采样频率之间的分频系数
	TTBIT.TIM_CounterMode = TIM_CounterMode_Up;					// 配置计数模式
	TTBIT.TIM_Period = arr;										// 配置计数上限值
	TTBIT.TIM_Prescaler = psc;									// 配置PSC分频系数
	TIM_TimeBaseInit(TIM3, &TTBIT);

	TOIT.TIM_OCMode = TIM_OCMode_PWM2;							// 选择PWM模式
	TOIT.TIM_OCNPolarity = TIM_OCPolarity_High;					// 极性选择为: 高电平有效
	TOIT.TIM_OutputState = TIM_OutputState_Enable;				// 使能输出
	TIM_OC2Init(TIM3, &TOIT);
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCFast_Enable);				// 使能TIM3的CCR2(通道2)预装载寄存器

	TIM_Cmd(TIM3, ENABLE);										// 使能TIM3
}

/**
 * @brief 实现PWM连接LED从而使led0能够vanish --> light 
 * @cond 初始化了MyPWM, 进入了while(1)循环
*/
void PWM_Flicker(void)
{
	int reset = 0;
	u16 count = 0;
	
	while(1){
		Delay_Myms(10);
		if(reset == 0){
			count ++;
		}else if(reset == 1){
			count --;
		}
		if(count > 300){
			reset = 1;
		}
		if(count == 0){
			reset = 0;
		}
		TIM_SetCompare2(TIM3, count);
	}
}

/**
 * @brief 定时器输入捕获初始化
 * @param arr [u16] period(周期)
 * @param psc [u16] prescaler(预分频系数)
*/
void Cap_MyInit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GIT;
	TIM_TimeBaseInitTypeDef TBTIT;
	NVIC_InitTypeDef NIT;
	TIM_ICInitTypeDef TIIT;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GIT.GPIO_Pin = GPIO_Pin_0;
	GIT.GPIO_Mode = GPIO_Mode_IPD;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GIT);
	

	TBTIT.TIM_ClockDivision = TIM_CKD_DIV1;					// F_dts = F_CKINT 采样频率 = 定时器输入频率 
	TBTIT.TIM_CounterMode = TIM_CounterMode_Up;
	TBTIT.TIM_Period = arr;
	TBTIT.TIM_Prescaler = psc;
	TIM_TimeBaseInit(TIM5, &TBTIT);

	TIIT.TIM_Channel = TIM_Channel_1;						// 设置捕获器通道 --> 定时器通道1
	TIIT.TIM_ICFilter = 0x00;								// 设置滤波器长度 --> 0
	TIIT.TIM_ICPolarity = TIM_ICPolarity_Rising;			// 设置捕获类型 --> 上升沿捕获
	TIIT.TIM_ICPrescaler = TIM_ICPSC_DIV1;					// 设置捕获分频系数 --> 不分频
	TIIT.TIM_ICSelection = TIM_ICSelection_DirectTI;		// 设置映射关系 --> 直接映射
	TIM_ICInit(TIM5, &TIIT);

	NIT.NVIC_IRQChannel = TIM5_IRQn;
	NIT.NVIC_IRQChannelCmd = ENABLE;
	NIT.NVIC_IRQChannelPreemptionPriority = 2;
	NIT.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NIT);

	TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);	// 使能更新中断(TIM_IT_Update) 和 捕获中断(TIM_IT_CC1)

	TIM_Cmd(TIM5, ENABLE);									// 使能定时器的计数器开关
}


/**
 * @brief 通过timer的捕获功能, 获取KEY_UP按键按下的时间, 并且打印到串口1上面
 * @cond 前提条件是开启了初始化了capture功能, 进入了while(1)循环
*/
void CAP_KEY()
{	
		long int temp = 0;
		Delay_Myms(10);
		TIM_SetCompare2(TIM3, TIM_GetCapture2(TIM3) + 1);		// 设置TIM3捕获比较寄存器2的值为 当前捕获比较寄存器的值 + 1

		if(TIM_GetCapture2(TIM3) == 300){						// 获取TIM3捕获比较寄存器2的值, 假设该值 = 300
			TIM_SetCompare2(TIM3, 0);							// 设置TIM3捕获比较寄存器2的值 = 0;
		}
		if(CAPTURE_STA & 0x80){
			temp = CAPTURE_STA & 0x3f;
			temp *= 65536;
			temp += CAPTURE_VAL;
			printf("HIGH: %ld us \r\n", temp);
			CAPTURE_STA = 0;
		}
}
 
