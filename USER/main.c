/****************************************************************************************************
@author: TuRan
@data: 2022/3/22
@des: 毕业设计主要框架
****************************************************************************************************/
#include "stdio.h"
#include "timer.h"
#include "DHT11.h"
#include "exti.h"
#include "key.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "LCD.h"
#include "tftlcd.h"
#include "tpad.h"
#include "adc.h"
#include "usart.h"
#include "beep.h"


volatile u32 threshold_temp = 99; 
volatile u32 threshold_humi = 0;

volatile u32 temperature = 34;
volatile u32 humidity = 23;
volatile u32 carbon = 17;


void Modual_Init_LCD(void);


int main(void)
{
	Stm32_Clock_Init(9);								// 初始化系统时钟设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		// 初始化NVIC优先级分组
	delay_init(72);										// 初始化延迟函数
	LED_Init();		  									// 初始化与LED连接的硬件接口
	Beep_Init();										// 初始化蜂鸣器
	KEY_Init();											// 初始化按键
	LCD_Init();			   								// 初始化LCD  
	EXTI_MyInit();										// 初始化PA0(KEY_UP), PE4(KEY_0) 外部中断
	TPAD_Init(6);										// 初始化电容触摸按键
	DHT11_Init();
	Usart_Init_USART1(921600);							// 初始化串口1, 波特率为115200


	Modual_Init_LCD();


	while(1){
		LCD_SetColor_Ground(BLUE, WHITE);
		LCD_ShowxNum(0, 40, temperature, 4, 16, 0);
		LCD_ShowxNum(0, 120, humidity, 4, 16, 0);
		LCD_ShowxNum(0, 197, carbon, 4, 16, 0);
		LCD_ShowxNum(180, 240-17, threshold_temp, 3, 16, 0);
		LCD_ShowxNum(210, 240-17, threshold_humi, 3, 16, 0);

		DHT11_Read_Data((u8 *)&temperature, (u8 *)&humidity);

		Beep_Alarm(temperature, humidity);		
	}
}



void Modual_Init_LCD(void)
{
	// 设置LCD底色
	LCD_SetColor_Ground(RED, WHITE);	
	LCD_Clear(DARK);
	LCD_ShowString(0, 0, 240, 32, 32, "temperature");
	LCD_ShowString(0, 80, 240, 32, 32, "humidity");
	LCD_ShowString(0, 157, 240, 32, 32, "carbon");
}