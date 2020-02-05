#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "ADF4351.h"

int main(void)
{
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168);
    uart_init(115200);
    LED_Init();
    ADF_Init();
    printf("Begin!\r\n");
	ADF4351WriteFreq(250);
	while(1) 
	{
        LED0 = !LED0;
		LED1 = !LED1;
        delay_ms(250);
	
	}	 
	
}

