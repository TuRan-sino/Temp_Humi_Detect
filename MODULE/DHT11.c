/****************************************************************************************************
@author: TuRan
@data: 
@des:   DHT11代码
		DHT11的data引脚 -------------- PG0
****************************************************************************************************/
#include "dht11.h"
#include "delay.h"


/**
 * @brief 复位DHT11
 * @retval void
*/
void DHT11_Rst(void)
{
	DHT11_Set_OUTPUT(); // SET OUTPUT
	DHT11_PIN_OUT = 0; //拉低DQ
	delay_ms(20); //拉低至少18ms
	DHT11_PIN_OUT = 1; // DQ=1
	delay_us(30); //主机拉高20~40us
}


/**
 * @brief 等待DHT11的返回信息
 * @retval u8
*/
u8 DHT11_Check(void)
{
	u8 retry = 0;
	DHT11_MODE_IN(); // SET INPUT
	while (DHT11_DQ_IN && retry < 100) // DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};
	if (retry >= 100)
		return 1;
	else
		retry = 0;
	while (!DHT11_DQ_IN && retry < 100) // DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if (retry >= 100)
		return 1;
	return 0;
}


/**
 * @brief 从DHT11中读取一个位
 * @retval [u8] 1 / 0
*/
u8 DHT11_Read_Bit(void)
{
	u8 retry = 0;
	while (DHT11_DQ_IN && retry < 100) //等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry = 0;
	while (!DHT11_DQ_IN && retry < 100) //等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40); //等待40us
	if (DHT11_DQ_IN)
		return 1;
	else
		return 0;
}


/**
 * @brief 从DHT11中读取一个字节
 * @retval u8
*/
u8 DHT11_Read_Byte(void)
{
	u8 i, dat;
	dat = 0;
	for (i = 0; i < 8; i++) {
		dat <<= 1;
		dat |= DHT11_Read_Bit_Mine();
	}
	return dat;
}


/**
 * @brief 从DHT11中读取一次数据
 * @param [u8 *] temp: 温度
 * @param [u8 *] humi: 湿度
 * @retval 0: 正常 || 1: 不正常
*/
u8 DHT11_Read_Data(u8* temp, u8* humi)
{
	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if (DHT11_Check() == 0) {
		for (i = 0; i < 5; i++) //读取40位数据
		{
			buf[i] = DHT11_Read_Byte();
		}
		if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4]) {
			*humi = buf[0];
			*temp = buf[2];
		}
	} else
		return 1;
	return 0;
}

/**
 * @brief 初始化DHT11
 * @retval 1: 初始化失败 || 0: 初始化成功
*/
u8 DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); //使能PG端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // PG11端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure); //初始化IO口
	GPIO_SetBits(GPIOG, GPIO_Pin_0); // PG11 输出高

	DHT11_SendStart(); //复位DHT11
	return DHT11_Wait_Respond(); //等待DHT11的回应
}


// ****************************************************************************************************
/**
 * @brief 	将数据线的模式配置为输出模式
 * 			DHT11的模式为通用推挽输出模式, 最大速度位50MHZ
 * 			CNF[0] = 00		MODE[0] = 11
 * @retval void
*/
void DHT11_Set_OUTPUT()
{
	GPIOG->CRL &= 0xFFFFFFF0;
	GPIOG->CRL |= 3;
}


/**
 * @brief 	将数据线的模式配置为输入模式
 * 			CNF[0] = 10		MODE[0] = 00
 * 			DHT11的模式为上拉/下拉输入模式
 * @retval 
*/
void DHT11_Set_INPUT()
{
	GPIOG->CRL &= 0XFFFFFFF0;
	GPIOG->CRL |= 8;
}


/**
 * @brief DHT11 发送数据传输开始函数, 使DHT11复位, 重新进入发送时序
 * @retval void
*/
void DHT11_SendStart(void)
{
	DHT11_Set_OUTPUT();			// 进入发送时序, 主机首先进入t1
	DHT11_OUT = 0;				// 先将数据线拉低, 持续20ms
	delay_ms(20);
	DHT11_OUT = 1;				// 将DHT11拉高,持续40us
	delay_us(40);
}


/**
 * @brief 	等待DHT11的返回信息
 * 			(DHT11 拉低数据线40 ~ 50ms, 之后DHT11拉高数据线 40 ~ 50ms)
 * @retval [u8] TRUE: 表示返回成功	|	FALSE: 表示返回失败
*/
u8 DHT11_Wait_Respond(void)
{
	int count = 0;
	DHT11_Set_INPUT();					// 将DHT11的数据线设置为输入模式
	while(count < 100){					// 假设在100微秒内, dht11的数据线被拉低, 则进行下一步
		if(DHT11_IN == 0)	break;		// 假设没有被拉低, 则返回FLASE
		delay_us(1);
	}
	if(count >= 100){
		return FALSE;
	}
	count = 0;
	while(count < 100){					// 假设在100微秒内DHT11的数据线被拉高, 则返回TRUE
		if(DHT11_IN == 1) 	break;
		delay_us(1);
	}
	if(count >= 100) return FALSE;		// 否则返回FALSE
	else return TRUE;
}


/**
 * @brief 	DHT11 初始化函数
 * 			初始化时先将PG0设置为输出模式, 以便开始向data口发送开始信号
 * @retval 
*/
void DHT11_Init_Mine(void)
{
	GPIO_InitTypeDef GIT;
	GIT.GPIO_Mode = GPIO_Mode_Out_PP;		// 设置为输出模式: 推挽输出
	GIT.GPIO_Pin = GPIO_Pin_0;
	GIT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GIT);

	GPIO_SetBits(GPIOG,GPIO_Pin_0);			// 起始时信号线拉高
	DHT11_SendStart();							// 宿主机开始向DHT11发送开始信号
}


// void DHT11_Wait_Low()
// {
// 	int count = 0;
// 	while(count < 100){
// 		count ++;
// 		delay_us(1);
// 	}
// }


/**
 * @brief 	DHT11接收一个位的函数
 * 			位0 : DHT11 被拉低14us紧接着被拉高28us之后被拉低
 * 			位1 : DHT11 被拉低14us紧接着被拉高100us之后被拉低
 * @retval 
*/
u8 DHT11_Read_Bit_Mine(void)
{
	int count = 0;
	while(count < 100){
		delay_us(1);
		if(DHT11_IN == 0) break;
		count ++;
	}
	count = 0;
	while(count < 100){
		delay_us(1);
		if(DHT11_IN == 1) break;
		count ++;
	}

	// 以上进行完成之后, DHT11完成了基本的开始时序

	delay_us(40);						// 延迟40us
	if(DHT11_IN == 1) return 1;			// 假设此时被数据线被拉高则说明是0, 否则是1
	else return 0;							
}


/**
 * @brief DHT11 读一个字节函数
 * @retval 当前dht11读的字节
*/
u8 DHT11_Read_Byte_Mine(void)
{
	u8 data = 0;
	for(int i = 0; i < 8; i ++){
		data <<= 1;
		data |= DHT11_Read_Bit_Mine();
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
u8 DHT11_Read_Data_Mine(u8 *temp, u8 *humi)
{
	u8 data[5];
	for(int i = 0; i < 5; i ++){
		data[i] = DHT11_Read_Bit_Mine();
	}
	if(data[0] + data[1] + data[2] + data[3] == data[4]){
		*humi = data[0];
		*temp = data[2];
		return TRUE;
	}else	return FALSE;
}
