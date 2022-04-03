#ifndef __HC05_H__
#define __HC05_H__
#include "sys.h"

#define HC05_KEY PAout(4)
#define HC05_LED PAin(15)

typedef struct BLE_PACK{
	u8 head;
	u16 Ble_humi;
	u16 Ble_Temp;
	u8 verify;
	u8 tail;
}BLE_PACK;

void HC05_SendData(BLE_PACK data);
void HC05_Init();
u8 HC05_Cal_Verify(u32 temp, u32 humi);



#endif