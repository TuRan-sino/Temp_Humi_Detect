#include <stdio.h>
#include "usart.h"

#ifdef __GNUC__
int _write (int fd, char *pBuffer, int size)
{
    for (int i = 0; i < size; i++)  {  
		while((USART1->SR&0X40)==0);					//等待上一次串口数据发送完成
		USART_SendData(USART1, pBuffer[i]);				//写DR,串口1将发送数据
	}  
    return size;
}
#endif




