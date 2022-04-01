/****************************************************************************************************
@author: TuRan
@data: 
@des:   DHT11代码
		DHT11的data引脚 -------------- PG0
****************************************************************************************************/
#include "dht11.h"
#include "delay.h"


/**
 * @brief 	将数据线的模式配置为输出模式
 * 			DHT11的模式为通用推挽输出模式, 最大速度位50MHZ
 * 			CNF[0] = 00		MODE[0] = 11
 * @retval void
*/
void DHT11_Set_OUTPUT()
{
	GPIO_InitTypeDef GIT;
	GIT.GPIO_Mode = GPIO_Mode_Out_PP;
	GIT.GPIO_Pin = GPIO_Pin_0;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GIT);
	
}


/**
 * @brief 	将数据线的模式配置为输入模式
 * 			CNF[0] = 10		MODE[0] = 00
 * 			DHT11的模式为上拉/下拉输入模式
 * @retval 
*/
void DHT11_Set_INPUT()
{
	
	GPIO_InitTypeDef GIT;
	GIT.GPIO_Mode = GPIO_Mode_IPU;
	GIT.GPIO_Pin = GPIO_Pin_0;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GIT);
}


/**
 * @brief DHT11 发送数据传输开始函数, 使DHT11复位, 重新进入发送时序
 * @retval void
*/
void DHT11_Set_Start(void)
{
	DHT11_Set_OUTPUT();			// 进入发送时序, 主机首先进入t1
	DHT11_OUT = 0;				// 先将数据线拉低, 持续20ms
	Delay_ms(20);
	DHT11_OUT = 1;				// 将DHT11拉高,持续40us
	Delay_us(30);
}


/**
 * @brief 	等待DHT11的返回信息
 * 			(DHT11 拉低数据线40 ~ 50ms, 之后DHT11拉高数据线 40 ~ 50ms)
 * @retval [u8] TRUE: 表示返回成功	|	FALSE: 表示返回失败
*/
u8 DHT11_Set_Respond(void)
{
	int count = 0;
	DHT11_Set_INPUT();					// 将DHT11的数据线设置为输入模式
	while(count < 100){		
		if(DHT11_IN == 0) break;		// 假设在100微秒内, dht11的数据线被拉低, 则进行下一步
		Delay_us(1);
		count ++;
	}
	if(count >= 100) return FALSE;		// 如果100us内, dht11的数据线没有被拉低返回false
	
	count = 0;
	while(count < 100){		
		if(DHT11_IN == 1)	break;		// 假设在100微秒内DHT11的数据线被拉高, 则返回TRUE
		Delay_us(1);
		count ++;
	}

	if(count >= 100) return FALSE;		// 否则返回FALSE

	return TRUE;
}


/**
 * @brief 	DHT11接收一个位的函数
 * 			位0 : DHT11 被拉低14us紧接着被拉高28us之后被拉低
 * 			位1 : DHT11 被拉低14us紧接着被拉高100us之后被拉低
 * @retval 
*/
u8 DHT11_Read_Bit(void)
{
	int count = 0;
	while(count < 100){
		Delay_us(1);
		if(DHT11_IN == 0) break;
		count ++;
	}
	count = 0;
	while(count < 100){
		Delay_us(1);
		if(DHT11_IN == 1) break;
		count ++;
	}

	// 以上进行完成之后, DHT11完成了基本的开始时序

	Delay_us(40);						// 延迟40us
	if(DHT11_IN == 1) return 1;			// 假设此时被数据线被拉高则说明是0, 否则是1
	else return 0;							
}


/**
 * @brief DHT11 读一个字节函数
 * @retval 当前dht11读的字节
*/
u8 DHT11_Read_Byte(void)
{
	u8 data = 0;
	for(int i = 0; i < 8; i ++){
		data <<= 1;						// data想左移动一位
		data |= DHT11_Read_Bit();		// data的最低位与获取的DHT11的最低为相与
		
	}

	return data;
}


/**
 * @brief 	DHT11都数据函数
 * 			第一个字节: 湿度整数
 * 			第二个字节: 湿度小数
 * 			第三个字节: 温度整数
 * 			第四个字节: 温度小数
 * 			第五个字节: 校验位 --> 前四个字节之和
 * @param temp [u8 *] 指针, 指向温度温度 
 * @param humi [u8 *] 指针, 只想湿度
 * @retval 
*/
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
	u8 data[5];
	DHT11_Set_Start();
	if(DHT11_Set_Respond() == TRUE){
		for(int i = 0; i < 5; i ++){
			data[i] = DHT11_Read_Byte();
		}
		if(data[0] + data[1] + data[2] + data[3] == data[4]){
			*humi = data[0] + (data[1] * 0.01);
			*temp = data[2] + (data[3] * 0.01);
			// *humi = data[0];
			// *temp = data[2];
		}	
	}else return FALSE;

	return TRUE;
}


/**
 * @brief 	DHT11 初始化函数
 * 			初始化时先将PG0设置为输出模式, 以便开始向data口发送开始信号
 * @retval 
*/
void Dht11_Init(void)
{
	GPIO_InitTypeDef GIT;
	GIT.GPIO_Mode = GPIO_Mode_Out_PP;		// 设置为输出模式: 推挽输出
	GIT.GPIO_Pin = GPIO_Pin_0;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GIT);

	GPIO_SetBits(GPIOG,GPIO_Pin_0);			// 起始时信号线拉高
	DHT11_Set_Start();						// 宿主机开始向DHT11发送开始信号
}