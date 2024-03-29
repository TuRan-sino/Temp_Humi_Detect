/****************************************************************************************************
@author: TuRan
@data: 
@des: 	usart_rx --> PA10
		usart_tx --> PA9

****************************************************************************************************/
#include "usart.h"
#include "led.h"
#include "delay.h"



/**
 * @brief 	初始化串口1
 * 			usart_rx --> PA10
 * 			usart_tx --> PA9
 * @param serial [u16] 确定串口的编号
 * @param buterate [u32] 确定串口的波特率
 * @note 	配合HC05蓝牙进入USART中断
 * @retval void
*/
void Usart_Init_USART1(u32 buterate)
{
	GPIO_InitTypeDef GInit;
	USART_InitTypeDef UsInit;
	NVIC_InitTypeDef NVInit;

	// usart 属于复用功能, usart_rx = PA10 | usart_tx = PA9
	// 因此, 需要初始化usart1 和 GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// 配置相应的GPIO口 GPIOA9 (USART1_TX) 
	GInit.GPIO_Mode = GPIO_Mode_AF_PP;		// GPIO的模式 = 复用推挽输出
	GInit.GPIO_Pin = GPIO_Pin_9;			// GPIO的引脚 = 9
	GInit.GPIO_Speed = GPIO_Speed_10MHz;	// GPIO的速度 = 10MHZ
	GPIO_Init(GPIOA, &GInit);
	// GPIOA10 (USART_RX)
	GInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;// GPIO的模式 = 浮空输入
	GInit.GPIO_Pin = GPIO_Pin_10; 			// GPIO的引脚 = 10
	GInit.GPIO_Speed =GPIO_Speed_10MHz;		// GPIO的速度 = 10MHZ
	GPIO_Init(GPIOA, &GInit);

	// 配置对应的串口 (串口1)
	UsInit.USART_BaudRate = buterate;									// buterate(波特率) 为本函数的传参, 需要保持开发板与主机的波特率相同!
	UsInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 流控制 = 无
	UsInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 设置usartmode为 RX + TX
	UsInit.USART_Parity = USART_Parity_No;								// 奇偶校验位 = 无
	UsInit.USART_StopBits = USART_StopBits_1;							// 停止位 = 1位
	UsInit.USART_WordLength = USART_WordLength_8b; 						// 字长 = 8字节
	USART_Cmd(USART1, ENABLE);											// 使能串口
	USART_Init(USART1, &UsInit);										// 初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);						// 配置串口的中断信息 --> 开启串口接收中断

	// 配置NVIC中断优先级
	NVInit.NVIC_IRQChannel = USART1_IRQn;			// 设置中断通道
	NVInit.NVIC_IRQChannelCmd = ENABLE;				// 使能NVIC中断优先级
	NVInit.NVIC_IRQChannelPreemptionPriority = 1; 	// 配置抢占优先级
	NVInit.NVIC_IRQChannelSubPriority = 1;			// 配置响应优先级
	NVIC_Init(&NVInit);
}


/**
 * @brief 	初始化串口3
 * 			usart3_rdx --> PB11
 * 			usart3_tdx --> PB10
 * @param buterate [u32] 串口波特率
 * @retval void
*/
void Usart_Init_USART3(u32 buterate)
{
	NVIC_InitTypeDef NIT;
	GPIO_InitTypeDef GIT;
	USART_InitTypeDef UIT;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1ENR_USART3EN, ENABLE);

	USART_DeInit(USART3);

	// 配置PB11 usart3_rdx
	GIT.GPIO_Mode = GPIO_Mode_AF_PP;
	GIT.GPIO_Pin = GPIO_Pin_11;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIT);

	
	GIT.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GIT.GPIO_Pin = GPIO_Pin_10;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIT);

	UIT.USART_BaudRate = buterate;
	UIT.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UIT.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	UIT.USART_Parity = USART_Parity_No;
	UIT.USART_StopBits = USART_StopBits_1;
	UIT.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &UIT);

	USART_Cmd(USART3, ENABLE);											// 使能串口
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);						// 配置串口的中断信息 --> 开启串口接收中断

	NIT.NVIC_IRQChannel = USART3_IRQn;
	NIT.NVIC_IRQChannelCmd = ENABLE;
	NIT.NVIC_IRQChannelPreemptionPriority = 2;
	NIT.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NIT);	
}

void Usart_SendString(char *s, u8 length)
{
	for(int i = 0; i < length; i ++){
		USART_SendData(USART1, *s);
		s ++;
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	}
}
