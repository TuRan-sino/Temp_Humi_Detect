# ADC 代码
```C
int main(void)
{
	u16 adcx;
	float temp;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Stm32_Clock_Init(9);		// 系统时钟设置
	delay_init(72);				// 初始化延迟函数
	LCD_Init();			   		// 初始化LCD  
	LED_MyInit();		  		// 初始化与LED连接的硬件接口
	ADC_MyInit();				// 初始化ADC
	

	LCD_SetColor(BLUE, WHITE);
	
	LCD_Clear(DARK);
	 
	LCD_ShowString(0, 0, 240, 32, 32, "ADC value");
	LCD_ShowString(0, 150, 240, 32, 32, "Voltage value");

	while(1)
	{	
		LCD_SetColor(DARK, WHITE);
		adcx = Get_ADC_Average(ADC_Channel_1, 10);
		LCD_ShowxNum(0, 40, adcx, 4, 16, 0);


		temp = (float)adcx * (3.3 / 4096);
		adcx = temp;
		LCD_SetColor(DARK, WHITE);
		LCD_ShowxNum(0, 190, adcx, 1, 16, 0);


		LED0 = ~LED0;
		delay_ms(1000);
	}
}
```



# TFTLED代码
```C
// TFTLED代码----------------------------------------------------------------------------------------------------
void changenum(char *p, int temp);

int main(void)
{
	char tempturenum_c[10];	
	int tempturenum = 53;
	char humandity_c[2];
	int humandity = 21;
	Stm32_Clock_Init(9);		//系统时钟设置
	delay_init(72);
	LED_MyInit();		  		//初始化与LED连接的硬件接口
	LCD_Init();			   		//初始化LCD  
	

	changenum(tempturenum_c, tempturenum);
	changenum(humandity_c, humandity);
	
	LCD_Clear(DARK);
	 
	while(1)
	{
		// Display_ALIENTEK_LOGO(0, 0);
		POINT_COLOR = RED;
		BACK_COLOR = WHITE;
		LCD_ShowString(0, 100, 240, 32, 32, "TEMPERATURE");
		LCD_ShowString(0, 140, 240, 24, 24, tempturenum_c);

		POINT_COLOR = WHITE;
		BACK_COLOR = BLUE;
		LCD_ShowString(0, 170, 240, 16, 16, "HUMANDITY");
		LCD_ShowString(0, 200, 240, 12, 12, humandity_c);
		LED0 = ~LED0;
		delay_ms(1000);
	}
}

void changenum(char *p, int temp)
{
	int temp1 = temp / 10;
	int temp2 = temp % 10;	

	p[0] = (char)(temp1 + 48);
	p[1] = (char)(temp2 + 48);
}
```

# 按键捕获实验代码
```C
// 按键捕获实验示例代码--------------------------------------------------------------------------------------------
int main(void)
{
	Delay_MyInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Usart_MyInit(1, 115200);
	LED_MyInit();
	PWM_MyInit(899, 0);
	Cap_MyInit(0xffff, 72 - 1);
	while(1){
		CAP_KEY();
	}

}

```

# PWM示例代码
```C
// PWM示例函数----------------------------------------------------------------------------------------------------
int main()
{
	LED_MyInit();
	Delay_MyInit();
	PWM_MyInit(899, 0);

	PWM_Vanish();
}
```

# 通用定时器示例代码
```C
// 通用定时器示例函数-----------------------------------------------------------------------------------------------
int main()
{
	u16 arr = 4999, psc = 7199;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_MyInit();
	Delay_MyInit();

	Time_MyInit(TIM3, arr, psc);	
	
	while(1);
}
```

# WDG示例代码
```C
// WWDG示例函数----------------------------------------------------------------------------------------------------
int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_MyInit();
 	LED_MyInit();
	KEY_MyInit();
	
	LED1 = LED_ON;
	Delay_ms(200);	  

	WWDG_MyInit(0X7F,0X5F,WWDG_Prescaler_8);
 	while(1)
	{
		LED1 = LED_OFF;		  	   
	}   
}
```

```C
IWDG实例函数----------------------------------------------------------------------------------------------------
int main()
{
	LED_MyInit();							// 初始化LED
	KEY_MyInit();							// 初始化KEY
	Delay_MyInit();							// 初始化DELAY


	Delay_ms(200);							// 显示延迟, 这个延迟的作用就是 : 在reset之前延迟200毫秒, 用于观测
	IWDG_MyInit(4, 625);					// 初始化IWDG
	LED0 = LED_ON;							// 在reset并且延迟200ms之后, 开启LED0

	while(1){
		if(KEY_Scan(0) == KEY_UP_PRESS){	// 假设当前key按下的是 WKUP按键
			IWDG_ReloadCounter();			// 喂狗 (不让程序reset)
		}
		Delay_ms(50);
	}
}
```

# EXTI示例代码
```C
// EXTI示例函数----------------------------------------------------------------------------------------------------
int main()
{
	Delay_MyInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Usart_MyInit(1, 115200);
	Beep_MyInit();	
	LED_MyInit();
	EXTI_MyInit();
	LED1 = LED_ON;
	LED0 = LED_ON;
	while(1){
		printf("OK \r\n");
		Delay_ms(1000);
	}
}
```

# LED示例代码
```C
// DEMO LED函数----------------------------------------------------------------------------------------------------
int main()
{
	Delay_Init();
	LED_Init();

	while(1){
		// LED0 = LED_ON;
		// LED1 = LED_ON;
		// Delay_ms(500);
		// LED0 = LED_OFF;
		// LED1 = LED_OFF;
		// Delay_ms(500);

		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		GPIO_SetBits(GPIOE, GPIO_Pin_5);
		Delay_ms(500);
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		Delay_ms(500);
	}
}
```

# USART示例代码
```C
// Demo USART函数----------------------------------------------------------------------------------------------------
int main(void)
{
	// u16 i = 0;
	// u32 count = 0;														// 发送计数器
	u16 t;
	u16 len;
	u16 times=0;
	Delay_MyInit();	    	 												// 延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 						// 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Usart_MyInit(1, 115200);	 											// 串口初始化为115200
	LED_MyInit();			     											// LED端口初始化
	KEY_MyInit();															// 初始化与按键连接的硬件接口
	 while(1){
		if(USART_RX_STA & 0x8000){											// 接收完成标记 = 1 --> 本次接收完成
			len = USART_RX_STA & 0x3fff;									// 得到此次接收到的数据长度
			printf("\r\nThe information you send is:\r\n");
			for(t=0;t<len;t++){
				USART_SendData(USART1, USART_RX_BUF[t]);					// 向串口1发送数据
				while(USART_GetFlagStatus(USART1 , USART_FLAG_TC) != SET);	// 返回值 != set --> 此时返回值仍处于 reset, 中断未完成. | 检验数据是否发送完成
			}
			printf("\r\n");
			USART_RX_STA=0;													// 重置 USART_RX_STA
		}else{		
			times++;
			if(times%200==0){
				printf("Please input the data, end with CR\r\n");
			}
			if(times%30==0)
				LED0 =! LED0;												//闪烁LED,提示系统正在运行.
			Delay_ms(10);
			// printf("Please wait for a min\r\n");
			// for(i = 0; i < 4; i ++){
			// 	Delay_ms(500);
			// }
		}
	}
}
```
