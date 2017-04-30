#include <STC15F2K60S2.H>
#include <intrins.h>
#include <NRF24L01.H>
#include <AD.H>			

//*********************************初始化A/D转换*************************************************
void adc_init()
{
	P1ASF=0x1f;
	ADC_RES=0;	 
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
	Delay(10); //适当延时
}
//**********************************获得A/D转换的数据********************************************
uint getADCResult(char ch)
{
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
	Delay(5);
	while (!(ADC_CONTR & ADC_FLAG));//等待转换完成
	ADC_CONTR &= ~ADC_FLAG; //关闭adc
	return ADC_RES;
}
