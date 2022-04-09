#include "iic.h"

int IIC_Init(void)
{
	GPIO_InitTypeDef GIT;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// 初始化IIC_SCL
	GIT.GPIO_Mode = GPIO_Mode_Out_PP;
	GIT.GPIO_Pin = GPIO_Pin_6;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIT);

	GIT.GPIO_Mode = GPIO_Mode_Out_PP;
	GIT.GPIO_Pin = GPIO_Pin_7;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIT);

	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);

	return TRUE;
}


/**
 * @brief IIC产生起始信号
 * @param 
 * @param 
 * @retval 
 * @note 
*/
void IIC_Send_Start(void)
{

}


void IIC_SDA_OUT()
{

}