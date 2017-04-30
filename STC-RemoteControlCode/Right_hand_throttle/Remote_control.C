//套件购买地址                http://shop112966725.taobao.com
//                                        STCunio

//**********************************遥控器程序Rev1.3正式版*************************
//声明：程序作者不会对本程序进行任何升级了，该程序就是该硬件的最终版本
//本程序可以自行二次开发，不过需要将开发后的版本上传到阿莫论坛，这样本程序开源才有意义，谢谢！
//          程序：SWUST 电气自动化13级 LQM

//          本程序适用于STC四轴的小日本手遥控器（右手油门）
//右手上下为油门，左右为横滚
//左手上下为俯仰，左右为旋转
//                                 MCU工作频率28MHZ！！！
#include <rtx51tny.h>
#include <STC15F2K60S2.H>
#include <NRF24L01.H>
#include <AD.H>
sbit RLED=P0^5;
sbit GLED=P0^6;
sbit LKEY=P4^7;
sbit RKEY=P3^4;

volatile int idata ay,ax,by,bx;
volatile int idata cy,cx,dy,dx;
volatile float idata battery;
unsigned char idata TxBuf[20]={0};
unsigned char idata RxBuf[20]={0};
void IO_and_Init();

void AD() _task_ 0 //获取AD采样数据
{
	IO_and_Init();   //初始化I/O口
	os_create_task (1);  //启动进程1
	for(;;)
	{
  	cy=getADCResult(3); //读取4个摇杆通道每个通道的8位数据，取值范围0-255
	Delay(10);
	cx=getADCResult(2);
	Delay(10);
	dy=getADCResult(1);
  	Delay(10);
	dx=getADCResult(0);
	Delay(10);
	battery=(getADCResult(4)*5.05*100)/256;   //电池电压检测通道 低于3.7V亮红灯
	Delay(10);
	if(battery<=370)
  {
	RLED=1;GLED=0;
	}
	else
	{
	RLED=0;GLED=1;
	}
	os_wait(K_IVL,3,0);//延时3个节拍
}
}
void NRF24L01()  _task_ 1
{	
	while(1)
	{
	 TxBuf[0]++;
	 TxBuf[1]=128;
	 TxBuf[2]=128;
	 TxBuf[3]=128;
	 if(cy<20) {goto EXIT;}//当油门拉至最低时遥控器解锁
	 if(RKEY==0) {TxBuf[5]=1;} else {TxBuf[5]=0;}
	 if(LKEY==0) {TxBuf[6]=1;} else {TxBuf[6]=0;}
	 nRF24L01_TxPacket(TxBuf);//发送TxBuf数组的数据
	 os_wait(K_IVL,2,0); //给一定延时让数据发送完成
	}
	EXIT:
	while(1)
	{
		if((dy-by)>=255){TxBuf[1]=255;}//用上电记录的数据对采样数据进行修正，保证摇杆中位时数据为128
		else if((dy-by)<=0){TxBuf[1]=0;}
		else{TxBuf[1]=dy-by;}
		
		if((dx-bx)>=255){TxBuf[3]=255;}
		else if((dx-bx)<=0){TxBuf[3]=0;}
		else{TxBuf[3]=dx-bx;}
		
		if((cx-ax)>=255){TxBuf[2]=255;}
		else if((cx-ax)<=0){TxBuf[2]=0;}
		else{TxBuf[2]=cx-ax;}
		//油门：TxBuf[4]
    //Yaw：TxBuf[3]
    //俯仰：TxBuf[1]
    //横滚：TxBuf[2]
		TxBuf[4]=cy;//油门通道不需处理，直接发送AD检测的8位数据即可，根据AD采样原理易知读取的AD采样数据不可能为负也不可能大于255
		if(RKEY==0) {TxBuf[5]=1;} else {TxBuf[5]=0;}
	  if(LKEY==0) {TxBuf[6]=1;} else {TxBuf[6]=0;}
		TxBuf[0]++;
	  nRF24L01_TxPacket(TxBuf);
		os_wait(K_IVL,2,0); //给一定延时让数据发送完成
	}
}
void IO_and_Init()
{
		P1M0=0x00;  //P1设为高阻模式
		P1M1=0xFF;
		P0M0=0Xff; //其他I/O口设置为准双向，弱上拉模式
		P0M1=0X00;
		P2M0=0X00;
		P2M1=0X00;
		P3M0=0X00;
		P3M1=0X00;
		P4M0=0X00;
		P4M1=0X00;
		P5M0=0Xff;
		P5M1=0X00;
		LKEY=1;  //拉高按键检测I/O口电平，按键为低电平触发
		RKEY=1;
		init_NRF24L01();  //初始化无线模块
    adc_init();        //初始化AD检测模块
	  ax=getADCResult(2)-128; //记录上电时摇杆的数据作为中位修正，因为摇杆中位要为128即256/2
	  Delay(10);
	  by=getADCResult(1)-128;
    Delay(10);
	  bx=getADCResult(0)-128;        //记录回中的轴的初始位置数据
	  Delay(10);
}