#include "Plus.h"
#include "ADF4351.h"
#include "usart.h"

void Write_Plus(double Fre)
{
    double temp;
    int flag;
    int FRAC,INT;
    int R_FRAC,R_INT; 
    int R0_Plus=0;
	
    temp=Fre-90;
	flag=temp*20*32;
	FRAC=flag%100;
	INT=flag/100;
	
    R_FRAC=0+FRAC;
    R_INT=0x240+INT;
	
    R0_Plus=R_INT; 
    R0_Plus<<=12;
    R0_Plus|=R_FRAC;
    R0_Plus<<=3;
	
	ADF_WriteData(R0_Plus);
	
}
