#ifndef __ADF4351_H
#define __ADF4351_H
#include "sys.h"
#include "delay.h"

/*
*
*   CE      ：芯片使能，高电平有效
*   LE      ：加载使能，高电平有效
*   CLK     ：串行时钟输入，上升沿有效
*   DATA    ：串行数据输入，串行数据以MSB优先方式加载，三个LSB用作控制位
*
**/

// 定义ADF_DATA串行数据线的操作方向
#define ADF_DATA_READ()  {GPIOG->MODER &= ~(3 << 13*2); GPIOG->MODER |= (0 << 13*2);}
#define ADF_DATA_WRITE() {GPIOG->MODER &= ~(3 << 13*2); GPIOG->MODER |= (1 << 13*2);}

// 位带操作
#define ADF_CE          PGout(14)
#define ADF_LE          PGout(15)
#define ADF_CLK         PGout(12)
// 注意数据线操作方向

#define ADF_DATA_OUT    PGout(13)
#define ADF_DATA_IN     PGin(13)

// GPIO初始化
void ADF_GPIO_Init(void);
void ADF_Init(void);
void WriteOneRegToADF4351(u32 Regster);
void ADF4351_Init_some(void);
void ADF4351WriteFreq(float Fre);		//	(xx.x) M Hz
#endif

