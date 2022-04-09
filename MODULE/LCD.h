#ifndef __LCD_H__
#define __LCD_H__
#include "sys.h"



//LCD的宽和高定义
#define LCD_TOTAL_BUF_SIZE	(240*240*2)
#define LCD_Buf_Size 1152
#define LCD_Width 	240
#define LCD_Height 	240

#define	LCD_PWR		PGout(8)
#define	LCD_RST		PGout(6)
#define	LCD_WR		PBout(14)
#define	LCD_CS		PGout(7)


extern u16	POINT_COLOR;	//默认画笔颜色
extern u16	BACK_COLOR;		//默认背景颜色



//画笔颜色
#define DARK			 0x0000
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 
#define BRRED 			 0XFC07 
#define GRAY  			 0X8430 
//GUI颜色

#define DARKBLUE      	 0X01CF	
#define LIGHTBLUE      	 0X7D7C
#define GRAYBLUE       	 0X5458
 
#define LIGHTGREEN     	 0X841F 
#define LIGHTGRAY        0XEF5B 
#define LGRAY 			 0XC618

#define LGRAYBLUE        0XA651 
#define LBBLUE           0X2B12

void Lcd_Spi_Send(u8 *data, u32 size);
void Lcd_Write_Data(u8 data);
void Lcd_Write_Cmd(u8 command);
void Lcd_Init(void);
void Lcd_Set_Ground(u32 foreground, u32 background);
void Lcd_Set_Address(u16 x_start, u16 y_start, u16 x_end, u16 y_end);
void Lcd_Set_Cmd(u8 command, u8 data);
void Lcd_Show_Char(u16 x, u16 y, u8 ch, u8 size);
void Lcd_Show_Variable(u16 x, u16 y, u32 num, u8 length, u8 size, u8 mode);
void Lcd_Show_String(u16 x, u16 y, u16 width, u16 height, u8 size, char *p);
void Lcd_Show_Halfword(const u16 da);
void Lcd_Show_Fill(u16 x_start, u16 y_start, u16 x_end, u16 y_end, u16 color);
void Lcd_Show_Clear(u16 color);
void Option_Shownumber_Temp();
void Option_Shownumber_Humi();
void Option_Modify_threshold(u8 choose);


#endif