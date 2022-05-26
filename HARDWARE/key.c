/****************************************************************************************************
@author: TuRan
@data: 
@des: 	KEY1 --> PE3
		KEY0 --> PE4
		KEY_UP --> PA0
****************************************************************************************************/
#include "key.h"
#include "delay.h"

/**
 * @brief 按键初始化函数, 将KEY0 -- KEY_UP 初始化
 * @param void
 * @retval void
*/
void KEY_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);	//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_3;						//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 								//设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);										//初始化GPIOE4,3

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 								//PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOA.0	
}

/**
 * @brief 	按键扫描函数
 * @retval	u8 (char): 返回扫描到按键的类型
*/
u8 KEY_Scan(u8 mode)
{
	static u8 key_state = 1;
	if(key_state == 1 && (KEY0 == 0 || KEY1 == 0 || KEY_UP == 1)){
		delay_ms(50);		// 去抖动函数
		key_state = 0;		// 设置 设置keystate = 0 即上一次有按键按下
		if(KEY0 == 0)
			return KEY0_PRESS;
		else if(KEY1 == 0)
			return KEY1_PRESS;
		else if(KEY_UP == 1)
			return KEY_UP_PRESS;
	}else if(KEY0 == 1 && KEY1 == 1 && KEY_UP == 0){
		key_state = 1;		// 设置keystate为1, 表示是啥过一次没有按键按下
	}
	
	return 0;			
}
