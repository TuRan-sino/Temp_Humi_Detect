#ifndef __LCD_H__
#define __LCD_H__
#include "tftlcd.h"
#include "key.h"
#include "led.h"

/**
 * @brief 更改字符串在LCD上面的颜色
 * @param foreground [u32] 前景色
 * @param background [u32] 背景色
 * @retval void 
*/
void LCD_SetColor_Ground(u32 foreground, u32 background);
void Option_Shownumber_Humi();
void Option_Shownumber_Temp();
void Option_Modify_threshold(u8 count);

#endif