#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "timer.h"
#include "LCD.h"
#include "tpad.h"
#include "beep.h"
#include "HC05.h"


extern u32 temperature;
extern u32 humidity;
extern u32 threshold_temp; 
extern u32 threshold_humi;


void USART1_IRQHandler(void) //串口1中断服务程序
{
	u8 res = 0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		res = USART_ReceiveData(USART1);
		if(res == 'q'){

		}else if(res == 'w'){

		}else if(res == 'd'){
			threshold_humi ++;
		}else if(res == 'f'){
			threshold_humi --;
		}else if(res == 'a'){
			threshold_temp ++;
		}else if(res == 's'){
			threshold_temp --;
		}else if(res == '0'){
			
		}
	}
}


/**
 * @brief EXTI线0中断函数, 用作至 key_up
 * @retval void
*/
void EXTI0_IRQHandler(void)
{	
	vu8 key = 0;
	u8 count = 0;
	u16 x_start = 0;
	u16 x_end = 240;

	BEEP = BEEP_OFF;	
	LED_Init();
	delay_Init();
	KEY_Init();
	LCD_Show_Clear(DARK);

	while(1){
		key = KEY_Scan(0);
		LCD_Set_Ground(DARK, WHITE);
		LCD_Show_String(60, 0, 240, 32, 32, "Option");
		LCD_Show_String(0, 50, 240, 32, 32, "Change Temp");
		LCD_Show_String(0, 100, 240, 32, 32, "Change Humi");
		if(key){
			switch(key){
				case KEY0_PRESS:
					if(count % 2 == 0){
						LCD_Show_Fill(x_start, 50, x_end, 82, RED);
						LCD_Show_Fill(x_start, 100, x_end, 132, DARK);
					}else{
						LCD_Show_Fill(x_start, 100, x_end, 132, RED);
						LCD_Show_Fill(x_start, 50, x_end, 82, DARK);
					}
					count ++;
					break;
				case KEY1_PRESS:
					if(count % 2 == 0){
						Option_Shownumber_Humi();
					}else{
						Option_Shownumber_Temp();
					}
			}
		}

		if(TPAD_Scan(0))	break;
	}

	LCD_Show_Clear(DARK);
	LCD_Set_Ground(RED, WHITE);	
	LCD_Show_Clear(DARK);
	LCD_Show_String(0, 0, 240, 32, 32, "temperature");
	LCD_Show_String(0, 80, 240, 32, 32, "humidity");
	LCD_Show_String(0, 157, 240, 32, 32, "carbon");

	EXTI_ClearITPendingBit(EXTI_Line0);
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){

		HC05_Init();
		delay_ms(500);
		

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}