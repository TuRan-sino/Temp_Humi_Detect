/****************************************************************************************************
@author: TuRan
@data: 
@des: 	蓝牙通信模块相关代码
		通过usart1连接开发板
		HC05_RXD(数据接受端口, 接开发板上面的TXD) --> PA10
		HC05_TXD(数据发送端口, 接开发板上面的RXD) --> PA9
****************************************************************************************************/
#include "HC05.h"
#include "usart.h"
#include "delay.h"


extern u32 temperature;
extern u32 humidity;


void HC05_SendData(BLE_PACK data)
{
	USART_SendData(USART1, data.head);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
	USART_SendData(USART1, data.Ble_Temp);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
	USART_SendData(USART1, data.Ble_humi);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
	USART_SendData(USART1, data.verify);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
	USART_SendData(USART1, data.tail);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
}



u8 HC05_Cal_Verify(u32 temp, u32 humi)
{
	u16 verify = 0;
	u8 res;

	verify = (u16)temp + (u16)humi;

	res = (u8)verify;

	return res;
}


void HC05_Init()
{
	BLE_PACK data;


	data.Ble_humi = humidity;
	data.Ble_Temp = temperature;
	data.head = 0XA5;
	data.tail = 0X5A;
	data.verify = HC05_Cal_Verify(temperature, humidity);


	HC05_SendData(data);
	
}
