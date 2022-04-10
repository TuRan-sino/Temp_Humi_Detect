/****************************************************************************************************
@author: TuRan
@data: 
@des: 	IIC相关代码
		SDA_OUT(数据线) --> 
		SCL(时钟线) --> 
****************************************************************************************************/
#include "iic.h"
#include "delay.h"

int IIC_Init(void)
{
	GPIO_InitTypeDef GIT;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// 初始化SCL
	GIT.GPIO_Mode = GPIO_Mode_Out_PP;
	GIT.GPIO_Pin = GPIO_Pin_6;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIT);

	// 初始化SDA
	GIT.GPIO_Mode = GPIO_Mode_Out_PP;
	GIT.GPIO_Pin = GPIO_Pin_7;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIT);

	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);

	return TRUE;
}


/**
 * @brief 	IIC发送起始信号
 * @note 	开始信号: SCL为高电平时, SDA从高电平 --> 低电平
*/
void IIC_Start(void)
{
	IIC_Set_SDA_OUT();

	SDA_OUT = 1;	// SDA起始位高电平
	SCL = 1;		// SCL为高电平
	delay_us(20);

	SDA_OUT = 0;	// SDA 从高电平 --> 低电平
	delay_us(20);

	SCL = 0;		// 设置SCL为0, 用于发送或者接收数据
}


/**
 * @brief	IIC 发送结束信号
 * @note 	结束信号: SCL为高点平时, SDA从低电平 --> 高电平
*/
void IIC_Stop(void)
{
	IIC_Set_SDA_OUT();

	SCL = 0;		// SCL起始为低电平
	SDA_OUT = 0;	// SDA起始位低电平
	delay_us(20);

	SCL = 1;		// SCL为高电平
	SDA_OUT = 1;	// SDA 从 低电平 --> 高电平, 发送结束信号
	delay_us(20);
}


/**
 * @brief	IIC接受应答信号
 * @note 	发送机每发送一个字节数据，然后释放总线，
 * 			由接收机反馈应答信号。应答信号为低电平时，规定为有效应答（ACK）
 * @retval	0: 返回ACK, 接受应答成功
 * 			1: 返回NACK, 接收应答失败
*/
u8 IIC_Wait_Ack(void)
{
	u8 wait = 0;

	IIC_Set_SDA_IN();			// 设置SDA线为输入模式

	SDA_OUT = 1;			// 将SDA置高
	delay_us(10);
	SCL = 1;				// 将SCL置高
	delay_us(10);
	while(SDA_IN == 1){		// 当SDA输入为高电平时进入循环, 等待SDA_IN为低电平(有效应答)
		wait ++;
		if(wait > 250){
			IIC_Stop();
			return 1;		// 超时, 返回NACK
		}
	}
	SCL = 0;
	return 0;				// 成功, 返回ACK
}


/**
 * @brief	IIC发送应答成功信号
 * @note	开发板作为接收机, 当发送机发送完成之后, 接收机需要返回ACK信号
 * 			应答信号为低电平时，规定为有效应答（ACK)
*/
void IIC_Send_Ack(void)
{
	IIC_Set_SDA_OUT();		// 将SDA设置为输出模式

	SCL = 0;			// 拉低SCL
	SDA_OUT = 0;		// 拉低SDA, 表示接收成功, 返回ACK信号(0)
	delay_us(20);

	// 进入检测时序, 检测SDA的高低
	SCL = 1;
	delay_us(20);
	SCL = 0;
}


/**
 * @brief	IIC发送非应答信号
 * @note	开发板作为接收机, 若开发板没有接收到数据, 返回NACK
 * 			应答信号为高电平的时候，规定为非应答（NACK)
*/
void IIC_Send_NAck(void)
{
	IIC_Set_SDA_OUT();		// 将SDA设置成输出模式

	SCL = 0;			// 拉低SCL
	SDA_OUT = 1;		// 拉高SDA, 表示接受失败, 返回NACK信号(1)
	delay_us(20);

	// 进入检测时序, 检测SDA的高低
	SCL = 1;		
	delay_us(20);
	SCL = 0;
}



/**
 * @brief 	设置SDA线为输出模式
 * @note	GPIO_mode: 推挽输出
*/
void IIC_Set_SDA_OUT()
{
	GPIOB->CRL &= 0X0FFFFFFF;
	GPIOB->CRL |= 8 << 28;
	// GPIOB->CRL |= 0X80000000;
}


/**
 * @brief 	设置SDA线为输入模式
 * @note	GPIO_mode: 浮空输入
*/
void IIC_Set_SDA_IN()
{
	GPIOB->CRL &= 0X0FFFFFFF;
	GPIOB->CRL |= 3 << 28;
	// GPIOB->CRL |= 0X30000000;
}


/**
 * @brief	IIC接收一个字节
 * @note	IIC总线传输数据时
 * 			时钟信号为高电平期间，数据线上的数据必须稳定
 * 			只有在时钟线上的信号为低电平期间，数据线上的电平信号允许改变
 * @param ack [u8] 表示应答信号的种类
 * @retval	receive [u16] IIC接收到的数据
*/
u16 IIC_Read_Byte(u8 ack)
{
	u16 receive = 0;
	IIC_Set_SDA_IN();					// 将SDA设置为输入模式

	for(int i = 0; i < 8; i ++){
		// 将SCL拉低拉高, 进入读数据模式
		SCL = 0;
		delay_us(20);
		SCL = 1;

		receive <<= 1;				// 最低为为传输的方向, 1表示主设备读取数据
		if(SDA_IN == 1){
			receive ++;
		}
		delay_us(20);
	}

	if(ack == 0)	IIC_Send_NAck();
	else if(ack == 1)	IIC_Send_Ack();
	return receive;
}


/**
 * @brief	IIC发送一个字节
 * @param 	txd [u8] 需要发送的字节
 * @note 	首先将时钟线拉低, 此时才能够传输数据
*/
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	IIC_Set_SDA_OUT();				// 将SDA设置为输出模式
	SCL = 0;

	for(t = 0; t < 8; t++){
		if((txd & 0x80) >> 7)		// txd的最高位为1 --> 输出1 | txd最高位为0 --> 输出0
			SDA_OUT = 1;
		else
			SDA_OUT = 0;

		txd <<= 1;					// 将TXD左移一位, 更新最高位
		delay_us(20);
		SCL = 1;
		delay_us(20);
		SCL = 0;
		delay_us(20);
	}
	delay_us(20);
}