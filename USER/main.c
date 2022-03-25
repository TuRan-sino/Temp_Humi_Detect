/****************************************************************************************************
@author: TuRan
@data: 2022/3/22
@des: 毕业设计主要框架
****************************************************************************************************/
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


void Sys_Init(void);


int main(void)
{

	u32 temperature = 34;
	u32 humidity = 23;
	u32 carbon = 17;

	Sys_Init();

	while(DHT11_Init())	
	{
		LCD_Clear(DARK);
		LCD_ShowString(30,130,200,16,16,"DHT11 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}	

	while(1){
		DHT11_Read_Data((u8 *)&temperature, (u8 *)&humidity);

		LCD_SetColor_Ground(BLUE, WHITE);
		LCD_ShowxNum(0, 40, temperature, 4, 16, 0);
		LCD_ShowxNum(0, 120, humidity, 4, 16, 0);
		LCD_ShowxNum(0, 197, carbon, 4, 16, 0);
		LCD_ShowxNum(180, 240-17, threshold_temp, 3, 16, 0);
		LCD_ShowxNum(210, 240-17, threshold_humi, 3, 16, 0);

		Beep_Alarm(temperature, humidity);

		delay_ms(500);
	}
}


void Sys_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		// 初始化NVIC优先级分组
	Stm32_Clock_Init(9);								// 初始化系统时钟设置
	delay_init(72);										// 初始化延迟函数
	LCD_Init();			   								// 初始化LCD  
	LED_Init();		  									// 初始化与LED连接的硬件接口
	ADC_MyInit();										// 初始化ADC
	Usart_MyInit(1, 115200);							// 初始化串口1, 波特率为115200
	TPAD_Init(6);										// 初始化电容触摸按键
	KEY_Init();											// 初始化按键
	EXTI_MyInit();										// 初始化PA0(KEY_UP), PE4(KEY_0) 外部中断
	Beep_Init();


	// 设置LCD底色
	LCD_SetColor_Ground(RED, WHITE);	
	LCD_Clear(DARK);
	LCD_ShowString(0, 0, 240, 32, 32, "temperature");
	LCD_ShowString(0, 80, 240, 32, 32, "humidity");
	LCD_ShowString(0, 157, 240, 32, 32, "carbon");
}
