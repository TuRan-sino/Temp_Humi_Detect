/****************************************************************************************************
@author: TuRan
@data: 
@des:	LCD相关代码
		基本上都是自己写的代码
		其中: 	option --> 表示多级菜单渲染代码
				LCD --> 表示tftlcd代码的自定义版本
****************************************************************************************************/
#include "LCD.h"
#include "tpad.h"


extern u32 threshold_temp;
extern u32 threshold_humi;


void Lcd_SetColor_Ground(u32 foreground, u32 background)
{	
	POINT_COLOR = foreground;
	BACK_COLOR = background;
}

void Option_Shownumber_Temp()
{	
	vu8 key = 0;
	u32 option_temp = 35;
	LCD_Clear(DARK);
	
	while(1){
		key = KEY_Scan(1);
		LCD_ShowString(0, 0, 240, 16, 16, "Please modify your num");
		LCD_ShowString(0, 16, 240, 32, 32, "key1 is inc");
		LCD_ShowString(0, 16 + 32, 240, 32, 32, "key0 is dec");

		LCD_ShowxNum(80, 120, option_temp, 4, 32, 0);
		switch(key){
			case KEY1_PRESS:
				option_temp ++;
				break;
			case KEY0_PRESS:
				option_temp --;
				break;
			case KEY_UP_PRESS:
				threshold_temp = option_temp;
				break;
		}


		if(TPAD_Scan(0)){
			LCD_Clear(DARK);
			break;
		}
	}
}

void Option_Shownumber_Humi()
{
	vu8 key = 0;
	u32 option_humi = 50;

	LCD_Clear(DARK);
	while(1){
		key = KEY_Scan(1);
		LCD_ShowString(0, 0, 240, 16, 16, "Please modify your num");
		LCD_ShowString(0, 16, 240, 32, 32, "key1 is inc");
		LCD_ShowString(0, 16 + 32, 240, 32, 32, "key0 is dec");
		LCD_ShowxNum(80, 120, option_humi, 4, 32, 0);

		switch(key){
			case KEY1_PRESS:
				option_humi ++;
				break;
			case KEY0_PRESS:
				option_humi --;
				break;
			case KEY_UP_PRESS:
				threshold_humi = option_humi;
				break;
		}
		
		if(TPAD_Scan(0)){
			LCD_Clear(DARK);
			break;
		}
	}
}



void Option_Modify_threshold(u8 choose)
{
	if(choose == 0){	
		LCD_Fill(0, 0, 240, 32, RED);
		LCD_Fill(0, 50, 240, 50 + 32, DARK);
		LCD_Fill(0, 100, 240, 100 + 32, DARK);		
		LCD_Fill(0, 150, 240, 150 + 32, DARK);	
		LCD_Fill(0, 200, 240, 200 + 32, DARK);
	}else if(choose == 1){
		LCD_Fill(0, 0, 240, 32, DARK);
		LCD_Fill(0, 50, 240, 50 + 32, RED);
		LCD_Fill(0, 100, 240, 100 + 32, DARK);		
		LCD_Fill(0, 150, 240, 150 + 32, DARK);	
		LCD_Fill(0, 200, 240, 200 + 32, DARK);
	}else if(choose == 2){
		LCD_Fill(0, 0, 240, 32, DARK);
		LCD_Fill(0, 50, 240, 50 + 32, DARK);
		LCD_Fill(0, 100, 240, 100 + 32, RED);		
		LCD_Fill(0, 150, 240, 150 + 32, DARK);	
		LCD_Fill(0, 200, 240, 200 + 32, DARK);
	}else if(choose == 3){
		LCD_Fill(0, 0, 240, 32, DARK);
		LCD_Fill(0, 50, 240, 50 + 32, DARK);
		LCD_Fill(0, 100, 240, 100 + 32, DARK);		
		LCD_Fill(0, 150, 240, 150 + 32, RED);	
		LCD_Fill(0, 200, 240, 200 + 32, DARK);
	}else if(choose == 4){
		LCD_Fill(0, 0, 240, 32, DARK);
		LCD_Fill(0, 50, 240, 50 + 32, DARK);
		LCD_Fill(0, 100, 240, 100 + 32, DARK);		
		LCD_Fill(0, 150, 240, 150 + 32, DARK);	
		LCD_Fill(0, 200, 240, 200 + 32, RED);
	}else if(choose > 4){
		LCD_Fill(0, 0, 240, 32, DARK);
		LCD_Fill(0, 50, 240, 50 + 32, DARK);
		LCD_Fill(0, 100, 240, 100 + 32, DARK);		
		LCD_Fill(0, 150, 240, 150 + 32, DARK);	
		LCD_Fill(0, 200, 240, 200 + 32, DARK);
	}
}