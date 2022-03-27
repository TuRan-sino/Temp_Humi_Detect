# 项目名称
基于STM32的车内温湿度检测系统<br>
Temperature and humidity detection system based on STM32

# 选材
1. 主控芯片<br>`STM32F103ZET6`
2. 温湿度检测模块<br>`DHT-11`
3. 图形模块<br>`TFTLCD`
4. 蓝牙模块<br>`HC-05`

# 接线
1. DHT11&emsp;&emsp;-->&emsp;&emsp;PG0<br>
2. TFTLCD_PWR&emsp;&emsp;-->&emsp;&emsp;PG8<br>TFTLCD_RST&emsp;&emsp;-->&emsp;&emsp;PG6<br>TFTLCD_WR&emsp;&emsp;-->&emsp;&emsp;PB14<br>TFTLCD_CS&emsp;&emsp;-->&emsp;&emsp;PG7<br>

# 简介
本项目采用了正点原子的`STM32F103ZET6`开发板, 利用SPI总线协议连接`TFTLCD`模块.<br>
`HC05`蓝牙模块的`TXD`连接单片机`USART_RXD`, `RXD`连接单片机的`USART_TXD`. 通过串口协议使得手机APP端能够与单片机通信<br>
`DHT11`通过单总线通信协议与单片机相连, `DHT11`的`DATA`引脚连接单片机的`PG0`口<br>

# Feature
通过TFTLCD与单片机相连, 使得单片机与操作者之间有一定的人机交互功能, TFTLCD中有三级菜单, 通过KEY_UP可以进入EXTI中断从而进入菜单. 进入菜单后可以对DHT11的温度阈值以及湿度阈值做出一定的控制<br>
通过HC05与单片机相连, 从而使得操作者能够使用手机开发板, 手机向HC05输出数据, 单片机进入USART中断, 从而对手机输入的数据进行响应. 比如更改温湿度阈值, 查看当前的温湿度等

# TODO
- [x] HC-05蓝牙模块
- [x] DHT11温湿度模块
- [x] TFTLCD模块
- [x] 交互系统
- [ ] 微信小程序 / 云服务器查看温度

