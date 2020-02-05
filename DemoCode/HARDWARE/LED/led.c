#include "led.h" 

//初始化PD12和PD13为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{    	 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);                     //使能GPIOD时钟

    //GPIOD12,D13初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                           //输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                          //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                      //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                            //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);                                  //初始化
	
    GPIO_SetBits(GPIOD,GPIO_Pin_13);                                        //GPIOD12设置高，灯灭
    GPIO_ResetBits(GPIOD,GPIO_Pin_12);                                      //GPIOD13设置低，灯灭

}
