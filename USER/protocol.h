#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "sys.h"


extern u32 temperature;
extern u32 humidity;

typedef struct DataPackage{
	char head;
	int data_temperature;
	int data_humidity;
	char verification;
	char tail;
}DataPackage;


DataPackage Protocol_Init();
void Transform_u16(u16 i, u8 *byte);
void Protocol_Send(DataPackage data);


#endif