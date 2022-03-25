#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "wwdg.h"
#include "timer.h"
#include "tftlcd.h"
#include "LCD.h"
#include "tpad.h"
#include "beep.h"


extern u8  CAPTURE_STA;
extern u16	CAPTURE_VAL;

/**
 * @brief 串口中断函数
 * @retval void
*/
void USART1_IRQHandler(void)
{
	u8 res;															// 定义: 接收暂存器
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){			// USART RXNE (接收中断) 中断发生(!= RESET (0) --> set (1))
		res = USART_ReceiveData(USART1);							// 将USART1的数据传送给res
		if((USART_RX_STA & 0x8000) == 0){							// USART_RX_STA 第15位为0 --> 未接收完成
			if(USART_RX_STA & 0x4000){								// USART_RX_STA 第14位为1 --> 之前接收到了0X0D标志
				if(res != 0x0a)										// res = 0x0a --> 表示上次接受错误, 重新开始
					USART_RX_STA = 0;								// 重置STA寄存器
				else
					USART_RX_STA |= 0x8000;							// 将STA寄存器的bit15置1, 表示上次接收完成
			}else{													// 之前未接收到0X0D标志位
				if(res == 0x0d)										// 本次接收到了0X0D标志位
					USART_RX_STA |= 0x4000;							// 将STA寄存器的bit14置1
				else{												// 本次暂时未接收到0X0D标志位
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = res;		// 本次接收还没有开始 --> 将res传输至BUF缓存器中
					USART_RX_STA ++;								// STA寄存器 0~13位 ++
					if(USART_RX_STA > (USART_REC_LEN - 1))			// 报错 --> 重置STA寄存器, 重新开始
						USART_RX_STA = 0;
				}
			}
		}
	}
}


// /**
//  * @brief EXTI线4中断函数
//  * @retval void
// */
// void EXTI4_IRQHandler(void)
// {
// 	// Delay_MyInit();
// 	Delay_Myms(10);
// 	if(KEY0 == KEY_ON){
// 		LED1 = !LED1;
// 	}
// 	EXTI_ClearITPendingBit(EXTI_Line4);
// }


/**
 * @brief EXTI线0中断函数, 绑定至 key_up
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
	delay_init(72);
	KEY_Init();
	LCD_Clear(DARK);

	while(1){
		key = KEY_Scan(0);
		LCD_SetColor_Ground(DARK, WHITE);
		LCD_ShowString(60, 0, 240, 32, 32, "Option");
		LCD_ShowString(0, 50, 240, 32, 32, "Change Temp");
		LCD_ShowString(0, 100, 240, 32, 32, "Change Humi");
		if(key){
			switch(key){
				case KEY0_PRESS:
					if(count % 2 == 0){
						LCD_Fill(x_start, 50, x_end, 82, RED);
						LCD_Fill(x_start, 100, x_end, 132, DARK);
					}else{
						LCD_Fill(x_start, 100, x_end, 132, RED);
						LCD_Fill(x_start, 50, x_end, 82, DARK);
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

	LCD_Clear(DARK);
	LCD_SetColor_Ground(RED, WHITE);	
	LCD_Clear(DARK);
	LCD_ShowString(0, 0, 240, 32, 32, "temperature");
	LCD_ShowString(0, 80, 240, 32, 32, "humidity");
	LCD_ShowString(0, 157, 240, 32, 32, "carbon");

	EXTI_ClearITPendingBit(EXTI_Line0);
}


/**
 * @brief WWDG中断函数 
 * @retval void
 */
void WWDG_IRQHandler(void)
{
	WWDG_SetCounter(WWDG_CNT);
	WWDG_ClearFlag();
	LED0 = !LED0;
}


/**
 * @brief 定时器3中断函数
 * @retval void
*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		LED1 = !LED1;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}


/**
 * @brief 定时器5中断函数
 * @retval void
*/
void TIM5_IRQHandler(void)
{
	if((CAPTURE_STA & 0x80) == 0){								// 当未检测到 捕获完成标志的时候
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){		// 检测到 更新终端的时候
			if(CAPTURE_STA & 0x40){								// 假设 检测到了高电平标志
				if((CAPTURE_STA & 0x3f) == 0x3f){				// 假设 捕获到的高电平次数多余bit 0 - 5
					CAPTURE_STA |= 0x80;						// 强制标记捕获完成标志, 并且将当前捕获计数器的值设置为最大
					CAPTURE_VAL = 0xffff;
				}else{
					CAPTURE_STA ++;								// 假设 高电平标志并没有超出上限, 将bit 0 - 5位 ++
				}
			}
		}

		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET){			// 检测 cc1捕获中断的时候 
			if(CAPTURE_STA & 0x40){								// 假设 当前检测到了下降沿 --> 检测到了跳变沿, 且并非更新终端, 且高电平标志 != 0
				CAPTURE_STA |= 0x80;							// 标志 捕获玩车个你标记
				CAPTURE_VAL = TIM_GetCapture1(TIM5);			// 将 捕获计数器的设设置为当前tim计数器的值
				TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising);		// 本次检测结束, 将边沿检测极性调整为 上升沿检测. 为下一次检测做准备
			}else{
				CAPTURE_VAL = 0;								// 检测到了上升沿
				CAPTURE_STA = 0;								// 表示本次程序还没有开始, 初始化所有寄存器
				TIM_SetCounter(TIM5, 0);
				CAPTURE_STA |= 0x40;
				TIM_OC1PreloadConfig(TIM5, TIM_ICPolarity_Falling);	// 为本次检测的结束做准备, 将边沿检测极性设置为 下降沿检测
			}
		}
	}

	TIM_ClearITPendingBit(TIM5, TIM_IT_Update | TIM_IT_CC1);	// 推出中断的时候, 需要重置中断检测标志
}