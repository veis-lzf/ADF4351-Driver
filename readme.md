# ADF4351 Driver

- EDA软件：Cadence Allegro 16.6

- 驱动程序使用的MCU：STM32F407VET6

- ADF4351 PCB说明：
  - 输出频率范围：35MHz~4400MHz
  - 注意：输出具有较多谐波，因为考虑到应用频率范围不同问题，板子上直接通过巴伦耦合输出，需要根据自己应用频率范围外接一个无源滤波器滤除谐波分量。
  
- 原理图

	![SCH1](/DesignFiles/Picture/SCH1.png)

	![SCH1](/DesignFiles/Picture/SCH2.png)

- PCB

	![ALL](/DesignFiles/Picture/ALL.png)

	![ALL](/DesignFiles/Picture/ADT.png)
