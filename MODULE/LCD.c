/****************************************************************************************************
@author: TuRan
@data: 
@des:	驱动芯片ST7789V2
		LCD相关代码
		尾缀描述
			option --> 表示多级菜单渲染代码
			LCD --> 表示tftlcd代码的自定义版本
		接线描述
			LCD_PWR(背光控制引脚) 			--> PG8
			LCD_RST(复位信号) 				--> PG6
			LCD_WR(写命令 / 写数据信号)  	--> PB14 (SPI2_MISO)
				WR = 0: 写命令
				WR = 1: 写数据
			LCD_CS(片选信号) 				--> PG7
			LCD_SDA(SDA信号线) 				--> PB15 (SPI2_MOSI)
			LCD_SCK(SCK时钟线)				--> PB13 (SPI2_SCK)
****************************************************************************************************/
#include "LCD.h"
#include "key.h"
#include "led.h"
#include "tpad.h"
#include "spi.h"
#include "font.h"


extern u32 threshold_temp;
extern u32 threshold_humi;

u16	POINT_COLOR = BLACK;	//画笔颜色	默认为黑色
u16	BACK_COLOR 	= WHITE;	//背景颜色	默认为白色

static u8 lcd_buf[LCD_Buf_Size];


/**
 * @brief 通过SPI想LCD底层发送数据
 * @param data [u8 *] 数据数组
 * @param size [u8] 数据长度
 * @retval void
*/
void LCD_Spi_Send(u8 *data, u32 size)
{
	u32 i;
	u32 delta;

	delta = size/0xFFFF;

	for(i = 0; i<=delta; i++){
		if( i==delta )
			SPI_Write(&data[i*0xFFFF], size%0xFFFF);
		
		else  
			SPI_Write(&data[i*0xFFFF], 0xFFFF);
	}
}


/**
 * @brief LCD写数据
 * @param data [u8] 数据内容
 * @retval void
*/
void LCD_Write_Data(u8 data)
{
	// WR置1, 进入写数据模式
	LCD_WR = 1;
	
	LCD_Spi_Send(&data, 1);
}


/**
 * @brief LCD写命令
 * @param command [u8] 命令内容
 * @retval void
*/
void LCD_Write_Cmd(u8 command)
{
	// WR置0, 进入写命令模式
	LCD_WR = 0;

	LCD_Spi_Send(&command, 1);
}



/**
 * @brief 静态函数, 仅在当前函数中生效. 计算幂函数
 * @param m [u8] 底数
 * @param n [u8] 幂
 * @retval 计算结果 
*/
static u32 POW(u8 m, u8 n)
{
	u32 result = 1;

	for(int i = n; i > 0; i --){
		result = result * m;
	}


	return result;
}


/**
 * @brief 初始化LCD函数
 * @retval void
*/
void LCD_Init(void)
{
	GPIO_InitTypeDef GIT;

	SPI_Init_SPI2();	
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

	delay_ms(120);
	LCD_RST = 1;
	
	delay_ms(120);
	LCD_Write_Cmd(0x11);
	delay_ms(120);


	LCD_Set_Cmd(0x36, 0x00);

	LCD_Set_Cmd(0x3a, 0x65);

	LCD_Write_Cmd(0xB2);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x33);
	LCD_Write_Data(0x33);

	LCD_Set_Cmd(0xB7, 0x72);

	LCD_Set_Cmd(0xbb, 0x3d);

	LCD_Set_Cmd(0xc0, 0x2c);

	LCD_Set_Cmd(0xc2, 0x01);

	LCD_Set_Cmd(0xc3, 0x19);

	LCD_Set_Cmd(0xc4, 0x20);

	LCD_Set_Cmd(0xc6, 0x0f);

	LCD_Write_Cmd(0xD0);
	LCD_Write_Data(0xA4);
	LCD_Write_Data(0xA1);

	LCD_Write_Cmd(0xE0);
	LCD_Write_Data(0xD0);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x0D);
	LCD_Write_Data(0x11);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x2B);
	LCD_Write_Data(0x3F);
	LCD_Write_Data(0x54);
	LCD_Write_Data(0x4C);
	LCD_Write_Data(0x18);
	LCD_Write_Data(0x0D);
	LCD_Write_Data(0x0B);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x23);

	LCD_Write_Cmd(0xE1);
	LCD_Write_Data(0xD0);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x11);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x2C);
	LCD_Write_Data(0x3F);
	LCD_Write_Data(0x44);
	LCD_Write_Data(0x51);
	LCD_Write_Data(0x2F);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x1F);
	LCD_Write_Data(0x20);
	LCD_Write_Data(0x23);

	LCD_Write_Cmd(0x21);

	LCD_Write_Cmd(0x29);

	LCD_Set_Address(0, 0, LCD_Width - 1, LCD_Height - 1);

	LCD_Show_Clear(WHITE);

	LCD_PWR = 1; 

}


/**
 * @brief 更改前景色背景色函数
 * @param foreground 前景色
 * @param background 背景色
 * @retval void
*/
void LCD_Set_Ground(u32 foreground, u32 background)
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
void LCD_Set_Address(u16 x_start, u16 y_start, u16 x_end, u16 y_end)
{
	LCD_Write_Cmd(0x2a);
	LCD_Write_Data(x_start >> 8);
	LCD_Write_Data(x_start);
	LCD_Write_Data(x_end >> 8);
	LCD_Write_Data(x_end);

	LCD_Write_Cmd(0x2b);
	LCD_Write_Data(y_start >> 8);
	LCD_Write_Data(y_start);
	LCD_Write_Data(y_end >> 8);
	LCD_Write_Data(y_end);

	LCD_Write_Cmd(0x2C);
}


/**
 * @brief 向ST7789写命令函数
 * @param command 命令格式
 * @param data 命令内容
 * @retval 
*/
void LCD_Set_Cmd(u8 command, u8 data)
{
	LCD_Write_Cmd(command);
	LCD_Write_Data(data);
}


/**
 * @brief 显示一个halfword (UN)
 * @param data halfword的内容
 * @retval void
*/
void LCD_Show_Halfword(const u16 data)
{
	u8 temp[2] = {0};

	temp[0] = data >> 8;
	temp[1] = data;

	LCD_WR = 1;
	LCD_Spi_Send(temp, 2);
}


/**
 * @brief 在指定位置显示一个字符 (UN)
 * @param x 该字符x轴的起始地址
 * @param y 该字符y轴的起始地址
 * @param ch 该字符的内容
 * @param size 该字符的字符大小
 * @retval void
*/
void LCD_Show_Char(u16 x, u16 y, u8 ch, u8 size)
{
	u8 temp, t1, t;
	u8 csize;															//得到字体一个字符对应点阵集所占的字节数
	u16 colortemp;
	u8 sta;

	ch = ch - ' '; 														//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

	if((x > (LCD_Width - size / 2)) || (y > (LCD_Height - size)))	return;

	LCD_Set_Address(x, y, x + size / 2 - 1, y + size - 1);				//(x,y,x+8-1,y+16-1)

	if((size == 16) || (size == 32) ){									//16和32号字体
		csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

		for(t = 0; t < csize; t++){
			if(size == 16)
				temp = asc2_1608[ch][t];								//调用1608字体
			else if(size == 32)
				temp = asc2_3216[ch][t];								//调用3216字体
			else 
				return;													//没有的字库

			for(t1 = 0; t1 < 8; t1++){
				if(temp & 0x80) 
					colortemp = POINT_COLOR;
				else 
					colortemp = BACK_COLOR;

				LCD_Show_Halfword(colortemp);
				temp <<= 1;
			}
		}
	}else if(size == 12){												//12号字体
		csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

		for(t = 0; t < csize; t++){
			temp = asc2_1206[ch][t];

			for(t1 = 0; t1 < 6; t1++){
				if(temp & 0x80) colortemp = POINT_COLOR;
				else colortemp = BACK_COLOR;

				LCD_Show_Halfword(colortemp);
				temp <<= 1;
			}
		}
	}else if(size == 24){		//24号字体
		csize = (size * 16) / 8;

		for(t = 0; t < csize; t++){
			temp = asc2_2412[ch][t];

			if(t % 2 == 0)sta = 8;
			else sta = 4;

			for(t1 = 0; t1 < sta; t1++){
				if(temp & 0x80) colortemp = POINT_COLOR;
				else colortemp = BACK_COLOR;

				LCD_Show_Halfword(colortemp);
				temp <<= 1;
			}
		}
	}	
}


/**
 * @brief 在某一个位置显示一个变量	(UN)
 * @param x X轴起始地址
 * @param y y轴起始地址
 * @param num 变量内容
 * @param length 变量的长度
 * @param size 变量的大小
 * @param mode 显示模式: 1 --> 高位不显示 | 0 --> 高位也显示
 * @retval 
*/
void LCD_Show_Variable(u16 x, u16 y, u32 num, u8 length, u8 size, u8 mode)
{
	u8 t, temp;
	u8 enshow = 0;

	for(t = 0; t < length; t++)
	{
		temp = (num / POW(10, length - t - 1)) % 10;

		if(enshow == 0 && t < (length - 1))
		{
			if(temp == 0)
			{
				if(mode)
					LCD_Show_Char(x + (size / 2)*t, y, '0', size);
				else
					LCD_Show_Char(x + (size / 2)*t, y, ' ', size);

				continue;
			}

			else enshow = 1;
		}

		LCD_Show_Char(x + (size / 2)*t, y, temp + '0', size);
	}
}


/**
 * @brief 在某一个位置显示一个字符串	(UN)
 * @param x x轴起始地址
 * @param y y轴起始地址
 * @param width 该字符串宽度
 * @param height 该字符串高度
 * @param size 该字符串的大小
 * @param p 字符串在内存中的地址
 * @retval void
*/
void LCD_Show_String(u16 x, u16 y, u16 width, u16 height, u8 size, char *p)
{
	u8 x0 = x;
	width += x;
	height += y;

	while((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
	{
		if(x >= width)
		{
			x = x0;
			y += size;
		}

		if(y >= height)break; //退出

		LCD_Show_Char(x, y, *p, size);
		x += size / 2;
		p++;
	}
}



/**
 * @brief 在某一个地方填满颜色块	(UN)
 * @param x_start x轴起始地址
 * @param y_start y轴起始地址
 * @param x_end x轴结束地址
 * @param y_end y轴结束地址
 * @retval
*/
void LCD_Show_Fill(u16 x_start, u16 y_start, u16 x_end, u16 y_end, u16 color)
{
	u16 i = 0;
	u32 size = 0, size_remain = 0;

	size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

	if(size > LCD_Buf_Size){
		size_remain = size - LCD_Buf_Size;
		size = LCD_Buf_Size;
	}

	LCD_Set_Address(x_start, y_start, x_end, y_end);

	while(1){
		for(i = 0; i < size / 2; i++){
			lcd_buf[2 * i] = color >> 8;
			lcd_buf[2 * i + 1] = color;
		}

		LCD_WR = 1;
		LCD_Spi_Send(lcd_buf, size);

		if(size_remain == 0) break;

		if(size_remain > LCD_Buf_Size){
			size_remain = size_remain - LCD_Buf_Size;
		}else{
			size = size_remain;
			size_remain = 0;
		}
	}
}





/**
 * @brief 清屏 (UN)
 * @param color 显示颜色
 * @retval void
*/
void LCD_Show_Clear(u16 color)
{
	u16 i, j;
	u8 data[2] = {0};

	data[0] = color >> 8;
	data[1] = color;

	LCD_Set_Address(0, 0, LCD_Width - 1, LCD_Height - 1);

	for(j = 0; j < LCD_Buf_Size / 2; j++)
	{
		lcd_buf[j * 2] =  data[0];
		lcd_buf[j * 2 + 1] =  data[1];

	}

	LCD_WR = 1;

	for(i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_Buf_Size); i++)
	{
		LCD_Spi_Send(lcd_buf, LCD_Buf_Size);
	}	
}


/**
 * @brief temp界面菜单
 * @retval void
*/
void Option_Shownumber_Temp()
{	
	vu8 key = 0;
	u32 option_temp = 35;
	LCD_Show_Clear(DARK);
	
	while(1){
		key = KEY_Scan(1);
		LCD_Show_String(0, 0, 240, 16, 16, "Please modify your num");
		LCD_Show_String(0, 16, 240, 32, 32, "key1 is inc");
		LCD_Show_String(0, 16 + 32, 240, 32, 32, "key0 is dec");

		LCD_Show_Variable(80, 120, option_temp, 4, 32, 0);
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
			LCD_Show_Clear(DARK);
			break;
		}
	}
}


/**
 * @brief humi(湿度)界面菜单
 * @retval void
*/
void Option_Shownumber_Humi()
{
	vu8 key = 0;
	u32 option_humi = 50;

	LCD_Show_Clear(DARK);
	while(1){
		key = KEY_Scan(1);
		LCD_Show_String(0, 0, 240, 16, 16, "Please modify your num");
		LCD_Show_String(0, 16, 240, 32, 32, "key1 is inc");
		LCD_Show_String(0, 16 + 32, 240, 32, 32, "key0 is dec");
		LCD_Show_Variable(80, 120, option_humi, 4, 32, 0);

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
			LCD_Show_Clear(DARK);
			break;
		}
	}
}


/**
 * @brief 修改阈值菜单页面
 * @param choose 当前游标处于的位置
 * @retval void 
*/
void Option_Modify_threshold(u8 choose)
{
	if(choose == 0){	
		LCD_Show_Fill(0, 0, 240, 32, RED);
		LCD_Show_Fill(0, 50, 240, 50 + 32, DARK);
		LCD_Show_Fill(0, 100, 240, 100 + 32, DARK);		
		LCD_Show_Fill(0, 150, 240, 150 + 32, DARK);	
		LCD_Show_Fill(0, 200, 240, 200 + 32, DARK);
	}else if(choose == 1){
		LCD_Show_Fill(0, 0, 240, 32, DARK);
		LCD_Show_Fill(0, 50, 240, 50 + 32, RED);
		LCD_Show_Fill(0, 100, 240, 100 + 32, DARK);		
		LCD_Show_Fill(0, 150, 240, 150 + 32, DARK);	
		LCD_Show_Fill(0, 200, 240, 200 + 32, DARK);
	}else if(choose == 2){
		LCD_Show_Fill(0, 0, 240, 32, DARK);
		LCD_Show_Fill(0, 50, 240, 50 + 32, DARK);
		LCD_Show_Fill(0, 100, 240, 100 + 32, RED);		
		LCD_Show_Fill(0, 150, 240, 150 + 32, DARK);	
		LCD_Show_Fill(0, 200, 240, 200 + 32, DARK);
	}else if(choose == 3){
		LCD_Show_Fill(0, 0, 240, 32, DARK);
		LCD_Show_Fill(0, 50, 240, 50 + 32, DARK);
		LCD_Show_Fill(0, 100, 240, 100 + 32, DARK);		
		LCD_Show_Fill(0, 150, 240, 150 + 32, RED);	
		LCD_Show_Fill(0, 200, 240, 200 + 32, DARK);
	}else if(choose == 4){
		LCD_Show_Fill(0, 0, 240, 32, DARK);
		LCD_Show_Fill(0, 50, 240, 50 + 32, DARK);
		LCD_Show_Fill(0, 100, 240, 100 + 32, DARK);		
		LCD_Show_Fill(0, 150, 240, 150 + 32, DARK);	
		LCD_Show_Fill(0, 200, 240, 200 + 32, RED);
	}else if(choose > 4){
		LCD_Show_Fill(0, 0, 240, 32, DARK);
		LCD_Show_Fill(0, 50, 240, 50 + 32, DARK);
		LCD_Show_Fill(0, 100, 240, 100 + 32, DARK);		
		LCD_Show_Fill(0, 150, 240, 150 + 32, DARK);	
		LCD_Show_Fill(0, 200, 240, 200 + 32, DARK);
	}
}