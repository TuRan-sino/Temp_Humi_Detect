#include "stdlib.h"
#include "protocol.h"
#include "usart.h"


void Protocol_Send(DataPackage data)
{
	printf("%c", data.head);
	printf("%d", data.data_humidity);
	printf("%d", data.data_temperature);
	printf("%c", data.verification);
	printf("%c", data.tail);
}


/**
 * @brief 将数据整合发送至数据包
 * @retval 
*/
DataPackage Protocol_Init()
{
	DataPackage *data;
	data = (DataPackage *)malloc(sizeof(DataPackage));

	data->head = 0XA5;
	data->data_humidity = humidity;
	data->data_temperature = temperature;
	data->verification = 0X00;
	data->tail = 0X5A;

	
	return *data;
}


void Transform_u16(u16 i, u8 *byte)
{
	byte[1] = (i & 0xFF00) >> 8;
	byte[0] = (i & 0xFF);
}