#ifndef __IIC_H__
#define __IIC_H__
#include "sys.h"


#define SCL PBout(6)
#define SDA_OUT PBout(7)
#define SDA_IN PBin(7)


int IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
u16 IIC_Read_Byte(u8 ack);
void IIC_Send_Ack(void);
void IIC_Send_NAck(void);
void IIC_Set_SDA_OUT();
void IIC_Set_SDA_IN();
void IIC_Send_Byte(u8 txd);


#endif