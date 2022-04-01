#ifndef __LCD_H__
#define __LCD_H__
#include "tftlcd.h"
#include "key.h"
#include "led.h"
#include "tpad.h"
#include "spi.h"


#define	LCD_PWR		PGout(8)
#define	LCD_RST		PGout(6)
#define	LCD_WR		PBout(14)
#define	LCD_CS		PGout(7)


void Lcd_Set_Cmd(u8 command, u8 data);
void Lcd_Set_Ground(u32 foreground, u32 background);
void Lcd_Spi_Send(u8 *data, u32 size);
void Lcd_Write_Data(u8 data);
void Lcd_Write_Cmd(u8 command);
void Lcd_Init(void);
void Option_Shownumber_Humi();
void Option_Shownumber_Temp();
void Option_Modify_threshold(u8 count);

#endif