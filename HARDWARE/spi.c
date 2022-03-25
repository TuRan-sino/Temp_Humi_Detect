#include "spi.h"

void SPI2_Init(void)
{
    RCC->APB2ENR |= 1 << 3; // PORTB时钟使能
    RCC->APB1ENR |= 1 << 14; // SPI2时钟使能
    //这里只针对SPI口初始化
    GPIOB->CRH &= 0X0F0FFFFF;
    GPIOB->CRH |= 0XB0B00000; // PB13/15复用
    GPIOB->ODR |= 1 << 13; // PB13/15上拉
    GPIOB->ODR |= 1 << 15; // PB15上拉

    SPI2->CR1 |= 0 << 10; //全双工模式
    SPI2->CR1 |= 1 << 9; //软件nss管理
    SPI2->CR1 |= 1 << 8;

    SPI2->CR1 |= 1 << 2; // SPI主机
    SPI2->CR1 |= 0 << 11; // 8bit数据格式
    SPI2->CR1 |= 1 << 1; //空闲模式下SCK为1 CPOL=1
    SPI2->CR1 |= 1 << 0; //数据采样从第二个时间边沿开始,CPHA=1
    //对SPI2属于APB1的外设.时钟频率最大为36M.
    SPI2->CR1 |= 0 << 3; // Fsck=Fpclk1/2
    SPI2->CR1 |= 0 << 7; // MSBfirst
    SPI2->CR1 |= 1 << 6; // SPI设备使能
    SPI2_ReadWriteByte(0xff); //启动传输
}

// SPI2速度设置函数
// SpeedSet:0~7
// SPI速度=fAPB1/2^(SpeedSet+1)
// APB1时钟一般为36Mhz
void SPI2_SetSpeed(u8 SpeedSet)
{
    SpeedSet &= 0X07; //限制范围
    SPI2->CR1 &= 0XFFC7;
    SPI2->CR1 |= SpeedSet << 3; //设置SPI2速度
    SPI2->CR1 |= 1 << 6; // SPI设备使能
}

// SPI2 读写一个字节
// TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u16 retry = 0;
    while ((SPI2->SR & 1 << 1) == 0) //等待发送区空
    {
        retry++;
        if (retry >= 0XFFFE)
            return 0; //超时退出
    }
    SPI2->DR = TxData; //发送一个byte
    retry = 0;
    while ((SPI2->SR & 1 << 0) == 0) //等待接收完一个byte
    {
        retry++;
        if (retry >= 0XFFFE)
            return 0; //超时退出
    }
    return SPI2->DR; //返回收到的数据
}

u8 SPI2_WriteData(u8* data, u16 size)
{
    u16 retry = 0;
    u16 i;

    for (i = 0; i < size; i++) {
        while ((SPI2->SR & 1 << 1) == 0) //等待发送区空
        {
            retry++;
            if (retry >= 0XFFFE)
                return 0; //超时退出
        }
        SPI2->DR = data[i]; //发送一个byte
        retry = 0;
    }

    return 1;
}
