/****************************************************************************************************
@author: TuRan
@data: 
@des: SPI相关代码
		SPI Flash: W251128
		SPI 接线:
			MISO: PB14
			MOSI: PB15
			SCK: PB13
		SPI 始终来源: 
			SPI的时钟来源于STM32的总线时钟APB2的预分频
		SPI 模式配置:
			SPI配置成主机模式
****************************************************************************************************/
#include "spi0.h"


void Spi_Init_Spi2()
{
	SPI_InitTypeDef SIT;
	GPIO_InitTypeDef GIT;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_SPI2EN, ENABLE);

	// 将PB3, PB14, PB15 全部设置为复用推挽输出
	GIT.GPIO_Mode = GPIO_Mode_AF_PP;
	GIT.GPIO_Pin = GPIO_Pin_13;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIT);
	GIT.GPIO_Mode = GPIO_Mode_AF_PP;
	GIT.GPIO_Pin = GPIO_Pin_14;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIT);
	GIT.GPIO_Mode = GPIO_Mode_AF_PP;
	GIT.GPIO_Pin = GPIO_Pin_15;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIT);
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	
	SIT.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		// 确定波特率分频
	SIT.SPI_CPHA = SPI_CPHA_2Edge;								// 确定CPHA的数值, 当前为: 第二个边沿采集
	SIT.SPI_CPOL = SPI_CPOL_High;								// 确定CPOL, 空闲时钟为高电平了, 当前为上升沿采集
	SIT.SPI_CRCPolynomial = 7;									// CRC校验
	SIT.SPI_DataSize = SPI_DataSize_8b;							// 确定当前数据位为8位
	SIT.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		// 确定当前工作方向: 全双工2线
	SIT.SPI_FirstBit = SPI_FirstBit_MSB;						// 确定从MSB(最高有效位)位开始
	SIT.SPI_Mode = SPI_Mode_Master;								// 确定主从模式, 当前为: 主模式
	SIT.SPI_NSS = SPI_NSS_Soft;									// 管理NSS软/ 硬
	SPI_Init(SPI2, &SIT);

	SPI_Cmd(SPI2, ENABLE);
	
	Spi2_Transmit(0XFF);
}


void Spi_Send()
{

}


bool Spi2_SetSpeed(u8 Prescaler)
{
	SPI2->CR1 &= 0xFFC7;
	SPI2->CR1 |= Prescaler;
	SPI_Cmd(SPI2, ENABLE);

	return TRUE;
}


/**
 * @brief 发送一个字节的同时读取一个字节
 * @param Data [u8]: 发送的字节内容
 * @retval 通过SPI2接收到的数据
*/
u8 Spi2_Transmit(u8 data_send)
{
	u8 data_receive;
	for(int i = 0; i < 200; i ++){
		if(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)		// 等待上一次的发送缓存为空, 当发送缓存位空时, 进入下一步
			break;
		if(i == 199) 
			return FALSE;
	}

	SPI_I2S_SendData(SPI2, data_send);										// SPI将数据发送出去的同时SPI也会接受数据
	
	for(int i = 0; i < 200; i ++){
		if(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)	break;	// SPI发送数据的同时会接收数据, 等待接受缓存结束
		if(i == 199)	return FALSE;
	}
	data_receive = SPI_I2S_ReceiveData(SPI2);

	return data_receive;
}