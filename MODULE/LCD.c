/****************************************************************************************************
@author: TuRan
@data: 
@des:	LCD相关代码
		尾缀描述
			option --> 表示多级菜单渲染代码
			LCD --> 表示tftlcd代码的自定义版本
		接线描述
			LCD_PWR(背光控制引脚) 			--> PG8
			LCD_RST(复位信号) 				--> PG6
			LCD_WR(写命令 / 写数据信号)  	--> PB14
				WR = 0: 写命令
				WR = 1: 写数据
			LCD_CS(片选信号) 				--> PG7
			LCD_SDA(SDA信号线) 				--> 
			LCD_SCK(SCK时钟线)
		
****************************************************************************************************/
#include "LCD.h"


extern u32 threshold_temp;
extern u32 threshold_humi;


/**
 * @brief 
 * @param 
 * @param 
 * @retval 
*/
void Lcd_Spi_Send(u8 *data, u32 size)
{
	u32 i;
	u32 delta;

	delta = size/0xFFFF;

	for(i = 0; i<=delta; i++){
		if( i==delta )
			Spi_Write(&data[i*0xFFFF], size%0xFFFF);
		
		else  
			Spi_Write(&data[i*0xFFFF], 0xFFFF);
	}
}


/**
 * @brief 
 * @param 
 * @param 
 * @retval 
*/
void Lcd_Write_Data(u8 data)
{
	// WR置1, 进入写数据模式
	LCD_WR = 1;
	
	Lcd_Spi_Send(&data, 1);
}


/**
 * @brief 
 * @param 
 * @param 
 * @retval 
*/
void Lcd_Write_Cmd(u8 command)
{
	// WR置0, 进入写命令模式
	LCD_WR = 0;

	Lcd_Spi_Send(&command, 1);
}


/**
 * @brief 
 * @param 
 * @param 
 * @retval 
*/
void Lcd_Init(void)
{
	GPIO_InitTypeDef GIT;

	Spi_Init_Spi2();	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);		// 初始化时钟

	// 初始化PG7, PG7, PG8
	GIT.GPIO_Mode = GPIO_Mode_Out_PP; 						// 推挽输出
	GIT.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GIT);

	// 初始化PB14
	GIT.GPIO_Mode = GPIO_Mode_Out_PP;
	GIT.GPIO_Pin = GPIO_Pin_14;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GIT);

	// 起初将所有端口置高
	GPIO_SetBits(GPIOG, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	
	LCD_CS = LCD_PWR = LCD_RST = 0;

	Delay_ms(120);
	LCD_RST = 1;
	
	Delay_ms(120);
	Lcd_Write_Cmd(0x11);
	Delay_ms(120);


	Lcd_Set_Cmd(0x36, 0x00);

	Lcd_Set_Cmd(0x3a, 0x65);

	Lcd_Write_Cmd(0xB2);
	Lcd_Write_Data(0x0C);
	Lcd_Write_Data(0x0C);
	Lcd_Write_Data(0x00);
	Lcd_Write_Data(0x33);
	Lcd_Write_Data(0x33);

	Lcd_Set_Cmd(0xB7, 0x72);

	Lcd_Set_Cmd(0xbb, 0x3d);

	Lcd_Set_Cmd(0xc0, 0x2c);

	Lcd_Set_Cmd(0xc2, 0x01);

	Lcd_Set_Cmd(0xc3, 0x19);

	Lcd_Set_Cmd(0xc4, 0x20);

	Lcd_Set_Cmd(0xc6, 0x0f);

	Lcd_Write_Cmd(0xD0);
	Lcd_Write_Data(0xA4);
	Lcd_Write_Data(0xA1);

	Lcd_Write_Cmd(0xE0);
	Lcd_Write_Data(0xD0);
	Lcd_Write_Data(0x04);
	Lcd_Write_Data(0x0D);
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x13);
	Lcd_Write_Data(0x2B);
	Lcd_Write_Data(0x3F);
	Lcd_Write_Data(0x54);
	Lcd_Write_Data(0x4C);
	Lcd_Write_Data(0x18);
	Lcd_Write_Data(0x0D);
	Lcd_Write_Data(0x0B);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x23);

	Lcd_Write_Cmd(0xE1);
	Lcd_Write_Data(0xD0);
	Lcd_Write_Data(0x04);
	Lcd_Write_Data(0x0C);
	Lcd_Write_Data(0x11);
	Lcd_Write_Data(0x13);
	Lcd_Write_Data(0x2C);
	Lcd_Write_Data(0x3F);
	Lcd_Write_Data(0x44);
	Lcd_Write_Data(0x51);
	Lcd_Write_Data(0x2F);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x1F);
	Lcd_Write_Data(0x20);
	Lcd_Write_Data(0x23);

	Lcd_Write_Cmd(0x21);

	Lcd_Write_Cmd(0x29);

	LCD_Address_Set(0, 0, LCD_Width - 1, LCD_Height - 1);

	LCD_Clear(WHITE);

	LCD_PWR = 1; 

}


void Lcd_Set_Ground(u32 foreground, u32 background)
{	
	POINT_COLOR = foreground;
	BACK_COLOR = background;
}


/**
 * @brief 设置数据写入LCD的缓存位置
 * @param x_start [u16]: X的起始坐标
 * @param y_start [u16]: y的起始坐标
 * @param x_end [u16]: x的终点坐标
 * @param y_end [u16]: y的终点坐标
 * @retval void
*/
void Lcd_Set_Address(u16 x_start, u16 y_start, u16 x_end, u16 y_end)
{
	
}


void Lcd_Set_Cmd(u8 command, u8 data)
{
	Lcd_Write_Cmd(command);
	Lcd_Write_Data(data);
}


/**
 * @brief 在指定位置显示一个字符
 * @param 
 * @param 
 * @retval 
*/
void Lcd_Show_Char(u16 x, u16 y, char ch, u8 size)
{
	
}


/**
 * @brief 
 * @param 
 * @param 
 * @retval 
*/
void Lcd_Show_Variable(u16 x, u16 y, u32 num, u8 length, u8 size)
{
	for(int i = 0; i < length; i ++){
		
	}
}


/**
 * @brief 
 * @param 
 * @param 
 * @retval 
*/
void Lcd_Show_String()
{

}


/**
 * @brief 
 * @param 
 * @param 
 * @retval 
*/
void Lcd_Show_Clear()
{
	
}


/**
 * @brief 
 * @param 
 * @param 
 * @retval 
*/
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


/**
 * @brief 
 * @param 
 * @param 
 * @retval 
*/
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


/**
 * @brief 
 * @param 
 * @param 
 * @retval 
*/
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