/****************************************************************************************************
@author: TuRan
@data: 
@des: 	数模转换单元相关代码
****************************************************************************************************/
#include "adc.h"
#include "delay.h"

/**
 * @brief 初始化adc1, 设置为多次转换, 右对齐, 不适用外部触发, 独立模式, 通道数量1, 不适用扫描模式
 * @param 
 * @param 
 * @retval void
*/
void ADC_MyInit(void)
{
	GPIO_InitTypeDef GIT;
	ADC_InitTypeDef AIT;

	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	GIT.GPIO_Mode = GPIO_Pin_1;
	GIT.GPIO_Pin = GPIO_Mode_AIN;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GIT);


	// 设置ADC的时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	// 复位ADC
	ADC_DeInit(ADC1);

	// 初始化ADC
	AIT.ADC_ContinuousConvMode = DISABLE;	// 设置是否为单次转换
	AIT.ADC_DataAlign = ADC_DataAlign_Right;	// 设置为右对齐
	AIT.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// 设置不使用外部触发
	AIT.ADC_Mode = ADC_Mode_Independent; 	// 设置独立模式
	AIT.ADC_NbrOfChannel = 1;	// 通道数量
	AIT.ADC_ScanConvMode = DISABLE;	// 不适用扫描模式
	ADC_Init(ADC1, &AIT);

	// 使能adc
	ADC_Cmd(ADC1, ENABLE);

	// 校准ADC
	ADC_ResetCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));	
}


/**
 * @brief 获取ADC1某一个通道的转换结果
 * @param 
 * @param 
 * @retval [u16] ADC的值
*/
u16 GET_ADC(u8 ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}


/**
 * @brief 通过调用get_adc函数, 取平均值
 * @param ch [u8] 
 * @param times [u8]
 * @retval [u16] 
*/
u16 Get_ADC_Average(u8 ch, u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for(t = 0; t < times; t ++){
		temp_val += GET_ADC(ch);
		Delay_ms(5);
	}
	return temp_val / times;
}