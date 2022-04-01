#include "key.h"
#include "delay.h"

/**
 * @brief 按键初始化函数, 将KEY0 -- KEY_UP 初始化
 * @param void
 * @retval void
*/
void Key_Init(void)
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
 * @param u8 mode(char): 确定按键扫描的模式
 * 		@arg:	mode = 1 --> 连续按键模式
 		@arg:	mode = 0 --> 非连续按键模式
 * @retval u8 (char): 返回扫描到按键的类型
*/
u8 KEY_Scan(u8 mode)
{
	// 定义了一个静态变量 key_state， 该变量能够判断上一次按键的状态
	// 0 --> 上一次按键为有效状态 | 1 --> 上一次按键为无效状态
	// 假设上一次按键状态为无效状态(1) --> 在mode = 0时 本次按键如果有效则认为有效
	// 假设上一次按键状态为有效状体(0) --> 在mode = 0时, 即便本次按键有效, 仍然认为其无效
	static u8 key_state = 1;
	if(mode == 1)		// 假设mode = 1 --> 连按模式, 设置key_state始终为1, 即上一次状态始终无效
		key_state = 1;
	if(key_state == 1 && (KEY0 == 0 || KEY1 == 0 || KEY_UP == 1)){
		Delay_ms(50);	// 去抖动函数
		key_state = 0;	// 设置本次函数过后的state为有效
		
		if(KEY0 == 0)
			return KEY0_PRESS;
		else if(KEY1 == 0)
			return KEY1_PRESS;
		else if(KEY_UP == 1)
			return KEY_UP_PRESS;
		
	}else if(KEY0 == 1 && KEY1 == 1 && KEY_UP == 0){
		key_state = 1;		// 此时没有任何按键被按下, 设置本次函数过后的state为无效
	}
	
	return 0;			
}
