#include "ADF4351.h"

#define ADF4351_R0			((uint32_t)0X2C8018)
#define ADF4351_R1			((uint32_t)0X8029)
#define ADF4351_R2			((uint32_t)0X10E42)
#define ADF4351_R3			((uint32_t)0X4B3)
#define ADF4351_R4			((uint32_t)0XEC803C)
#define ADF4351_R5			((uint32_t)0X580005)

#define ADF4351_R1_Base		((uint32_t)0X8001)
#define ADF4351_R4_Base		((uint32_t)0X8C803C)
#define ADF4351_R4_ON		((uint32_t)0X8C803C)
#define ADF4351_R4_OFF		((uint32_t)0X8C883C)

#define ADF4351_RF_OFF		((uint32_t)0XEC801C)

#define ADF4351_PD_ON		((uint32_t)0X10E42)
#define ADF4351_PD_OFF		((uint32_t)0X10E02)

uint8_t buf[4] = {0, 0, 0, 0};

// ADF4351写count字节数据函数，先写MSB
void ADF_Write(uint8_t count, uint8_t * reg)
{

	uint8_t ValueToWrite = 0;
	uint8_t i = 0;
	uint8_t j = 0;
	
    // 配置数据线方向为为主机输出状态，
    ADF_DATA_WRITE();
    // 使能芯片片选
	ADF_CE = 1;
	delay_us(1);
	ADF_CLK = 0;
	ADF_LE = 0;
	delay_us(1);
	
	for(i = count; i>0; i--)
	{
		ValueToWrite = *(reg + i - 1);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				ADF_DATA_OUT = 1;
			}
			else
			{
				ADF_DATA_OUT = 0;
			}
			delay_us(1);
			ADF_CLK = 1;
			delay_us(1);
			ValueToWrite <<= 1;
			ADF_CLK = 0;	
		}
	}
    // 使能数据加载
	ADF_LE = 1;
	delay_us(1);
	ADF_LE = 0;
    delay_us(1);
}


// I/O初始化
void ADF_GPIO_Init(void)
{  
    GPIO_InitTypeDef  GPIO_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    ADF_CE  = 0;    // 关闭芯片片选
    ADF_LE  = 0;    // 关闭数据加载
    ADF_CLK = 0;    // 拉低串行时钟线
}

// 单频输出
void ADF_Init()
{
    // 初始化GPIO
    ADF_GPIO_Init();
    
	buf[3] = 0x00;
	buf[2] = 0x58;
	buf[1] = 0x00;				// write communication register 0x00580005 to control the progress 
 	buf[0] = 0x05;				// to write Register 5 to set digital lock detector
	ADF_Write(4,buf);		   
								   
	buf[3] = 0x00;                 
	buf[2] = 0xec;				// (DB23=1)The signal is taken from the VCO directly;(DB22-20:4H)the RF divider is 16;(DB19-12:50H)R is 80
	buf[1] = 0x80;				// (DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				// (DB5=1)RF output is enabled;(DB4-3=3H)Output power level is 5
	ADF_Write(4,buf);		   
								   
	buf[3] = 0x00;                 
	buf[2] = 0x00;                 
	buf[1] = 0x04;				// (DB14-3:96H)clock divider value is 150.
 	buf[0] = 0xB3;                 
	ADF_Write(4,buf);	       
								   
	buf[3] = 0x00;                 
	buf[2] = 0x01;				// (DB6=1)set PD polarity is positive;(DB7=1)LDP is 6nS;
	buf[1] = 0x0E;				// (DB8=0)enable fractional-N digital lock detect;
 	buf[0] = 0x42;				// (DB12-9:7H)set Icp 2.50 mA;
	ADF_Write(4,buf);			// (DB23-14:1H)R counter is 1
								   
	buf[3] = 0x00;                 
	buf[2] = 0x00;                 
	buf[1] = 0x80;			   	// (DB14-3:6H)MOD counter is 6;
 	buf[0] = 0x29;			   	// (DB26-15:6H)PHASE word is 1,neither the phase resync 
	ADF_Write(4,buf);	   		// nor the spurious optimization functions are being used
								// (DB27=1)prescaler value is 8/9
								   
	buf[3] = 0x00;                 
	buf[2] = 0x2c;                 
	buf[1] = 0x80;                 
 	buf[0] = 0x18;				// (DB14-3:0H)FRAC value is 0;
	ADF_Write(4,buf);			// (DB30-15:140H)INT value is 320;
    
}

void WriteOneRegToADF4351(uint32_t Regster)
{
	uint8_t buf[4] = {0,0,0,0};
	buf[3] = (uint8_t)((Regster>>24)&(0X000000FF));
	buf[2] = (uint8_t)((Regster>>16)&(0X000000FF));
	buf[1] = (uint8_t)((Regster>>8) &(0X000000FF));
	buf[0] = (uint8_t)((Regster)&(0X000000FF));
	ADF_Write(4,buf);	
}
void ADF4351_Init_some(void)
{
	WriteOneRegToADF4351(ADF4351_R2);
	WriteOneRegToADF4351(ADF4351_R3);
	WriteOneRegToADF4351(ADF4351_R5);
}

void ADF4351WriteFreq(float Fre)		//	(xx.x) M Hz
{
	uint16_t Fre_temp, N_Mul = 1, Mul_Core = 0;
	uint16_t INT_Fre, Frac_temp, Mod_temp, i;
	uint32_t W_ADF4351_R0 = 0, W_ADF4351_R1 = 0, W_ADF4351_R4 = 0;
	float multiple;
	
	if(Fre < 35.0)
		Fre = 35.0;
	if(Fre > 4400.0)
		Fre = 4400.0;
	Mod_temp = 1000;
	Fre = ((float)((uint32_t)(Fre*10)))/10;
	
	Fre_temp = Fre;
	for(i = 0; i < 10; i++)
	{
		if(((Fre_temp*N_Mul) >= 2199.9) && ((Fre_temp*N_Mul) <= 4400.1))
			break;
		Mul_Core++;
		N_Mul = N_Mul*2;
	}
	
	multiple = (Fre*N_Mul)/25;		// 带宽出已固定为参考25M，若改参考频率，则应修改R4低字节
	INT_Fre = (uint16_t)multiple;
	Frac_temp = ((uint32_t)(multiple*1000))%1000;
	while(((Frac_temp%5) == 0) && ((Mod_temp%5) == 0))
	{
		Frac_temp = Frac_temp/5;
		Mod_temp = Mod_temp/5;
	}
	while(((Frac_temp%2) == 0)&&((Mod_temp%2) == 0))
	{
		Frac_temp = Frac_temp/2;
		Mod_temp = Mod_temp/2;
	}
	
	Mul_Core %= 7;
	W_ADF4351_R0 = (INT_Fre<<15)+(Frac_temp<<3);
	W_ADF4351_R1 = ADF4351_R1_Base + (Mod_temp<<3);
	W_ADF4351_R4 = ADF4351_R4_ON + (Mul_Core<<20);
	
	WriteOneRegToADF4351(ADF4351_RF_OFF);
	WriteOneRegToADF4351(W_ADF4351_R1);
	WriteOneRegToADF4351(W_ADF4351_R0);
	WriteOneRegToADF4351(W_ADF4351_R4);

}
