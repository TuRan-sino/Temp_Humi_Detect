#include <stdio.h>
#include "usart.h"

#ifdef __GNUC__
int _write (int fd, char *pBuffer, int size)
{
    for (int i = 0; i < size; i++)  {  
		USART_SendData(USART1, pBuffer[i]);				//写DR,串口1将发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	}  
    return size;
}
#endif