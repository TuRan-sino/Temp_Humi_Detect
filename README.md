# Project name
Temperature and humidity detection system based on STM32

# Project brief
1. DHT11&emsp;&emsp;-->&emsp;&emsp;PG0<br>
2. TFTLCD_PWR&emsp;&emsp;-->&emsp;&emsp;PG8<br>TFTLCD_RST&emsp;&emsp;-->&emsp;&emsp;PG6<br>TFTLCD_WR&emsp;&emsp;-->&emsp;&emsp;PB14<br>TFTLCD_CS&emsp;&emsp;-->&emsp;&emsp;PG7<br>

# Material selection
1. Carbon dioxide sensor<br>`SGP30`
2. Temperature and humidity sensor<br>`DHT-11`
3. LCD<br>`TFTLCD`
4. Bluetooth<br>`HC-05`

# Folder description
1. `CORE`:<br>CM3 Hardware drive code
1. `FWLIB`:<br>Peripheral drive code
1. `MODULE`:<br>Custom peripheral code
1. `BUILD`:<br>Files generated from `KEIL` and `arm-linux-gcc`
1. `SYSTEM`:<br>System drive code
1. `USER`:<br>User code

# TODO
- [ ] HC-05蓝牙模块
- [x] DHT11温湿度模块
- [x] TFTLCD模块
- [ ] SGP30二氧化碳模块
- [x] 交互系统
- [ ] 微信小程序 / 云服务器查看温度

# LOG
* 2022.3.36<br>基本上完善了所有的GTK以及温湿度传感系统
* 2022.3.22<br>搭建了基本框架, 基本上完善了GTK系统<br>

