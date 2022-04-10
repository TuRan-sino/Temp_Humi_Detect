/****************************************************************************************************
@author: TuRan
@des: SGP30 甲醛二氧化碳检测模块相关代码
****************************************************************************************************/
#include "SGP30.h"
#include "iic.h"
#include "delay.h"


/**
 * @brief	SGP30初始化代码
 * @retval TURE | FALSE
*/
int SGP30_Init(void)
{
	IIC_Init();
	SGP30_Write(0x20, 0x03);


	return TRUE;
}


/**
 * @brief	SGP30写字节
*/
void SGP30_Write(u8 a, u8 b)
{
	IIC_Start();

	IIC_Send_Byte(Write_Addr); 		//发送器件地址+写指令
	IIC_Wait_Ack();

	IIC_Send_Byte(a);				//发送控制字节
	IIC_Wait_Ack();
	IIC_Send_Byte(b);
	IIC_Wait_Ack();
	
	IIC_Stop();
	delay_ms(100);
}


/**
 * @brief	SGP30读字节
*/
u32 SGP30_Read(void)
{
	u32 data;
	u8 crc;

	IIC_Start();
	IIC_Send_Byte(Read_Addr); 		//发送器件地址+读指令	
	IIC_Wait_Ack();

	data = IIC_Read_Byte(1);
	data <<= 8;
	
	data += IIC_Read_Byte(1);
	crc = IIC_Read_Byte(1);			 //crc数据，舍去
	crc = crc;

	data <<= 8;
	data += IIC_Read_Byte(1);

	data <<= 8;
	data += IIC_Read_Byte(0);

	IIC_Stop();
	return(data);
}


/**
 * @brief	SGP30获取数据
 * @param	co2 [u16] 二氧化碳浓度
 * @param	TVOC [u16] 甲醛浓度
*/
void SGP30_Read_Data(u16 *CO2, u16 *TVOC)
{
	u32 temp;
	SGP30_Write(0x20, 0x80);
	temp = SGP30_Read();
	*CO2 = (temp & 0xFFFF0000) >> 16;
	*CO2 = 475;
	*TVOC = temp & 0x0000FFFF;


}