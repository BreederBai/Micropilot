//�׼������ַ                http://shop112966725.taobao.com
//                                        STCunio

//**********************************ң��������Rev1.3��ʽ��*************************
//�������������߲���Ա���������κ������ˣ��ó�����Ǹ�Ӳ�������հ汾
//������������ж��ο�����������Ҫ��������İ汾�ϴ�����Ī��̳������������Դ�������壬лл��
//          ����SWUST �����Զ���13�� LQM

//          ������������STC�����������ң�������������ţ�
//��������Ϊ���ţ�����Ϊ��ת
//��������Ϊ����������Ϊ���
//                                 MCU����Ƶ��28MHZ������
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

void AD() _task_ 0 //��ȡAD��������
{
	IO_and_Init();   //��ʼ��I/O��
	os_create_task (1);  //��������1
	while(1)
	{
  cy=getADCResult(1);   //��ȡ4��ҡ��ͨ��ÿ��ͨ����8λ���ݣ�ȡֵ��Χ0-255
	Delay(10);
	cx=getADCResult(0);
	Delay(10);
	dy=getADCResult(3);
  Delay(10);
	dx=getADCResult(2);
	Delay(10);
	battery=(getADCResult(4)*5.05*100)/256;   //��ص�ѹ���ͨ�� ����3.7V�����
	Delay(10);
	if(battery<=370)
  {
	RLED=1;GLED=0;
	}
	else
	{
	RLED=0;GLED=1;
	}
	os_wait(K_IVL,3,0);//��ʱ3������
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
	 if(cy<20) {goto EXIT;}   //�������������ʱң��������
	 if(RKEY==0) {TxBuf[5]=1;} else {TxBuf[5]=0;}
	 if(LKEY==0) {TxBuf[6]=1;} else {TxBuf[6]=0;}
	 nRF24L01_TxPacket(TxBuf);//����TxBuf���������
	 os_wait(K_IVL,2,0);  //��һ����ʱ�����ݷ������
	}
	EXIT:
	while(1)
	{
		if((dy-by)>=255){TxBuf[1]=255;}  //���ϵ��¼�����ݶԲ������ݽ�����������֤ҡ����λʱ����Ϊ128
		else if((dy-by)<=0){TxBuf[1]=0;}
		else{TxBuf[1]=dy-by;}
		
		if((dx-bx)>=255){TxBuf[2]=255;}
		else if((dx-bx)<=0){TxBuf[2]=0;}
		else{TxBuf[2]=dx-bx;}
		
		if((cx-ax)>=255){TxBuf[3]=255;}
		else if((cx-ax)<=0){TxBuf[3]=0;}
		else{TxBuf[3]=cx-ax;}
		//���ţ�TxBuf[4]
    //Yaw��TxBuf[3]
    //������TxBuf[1]
    //�����TxBuf[2]
		TxBuf[4]=cy;  //����ͨ�����账����ֱ�ӷ���AD����8λ���ݼ��ɣ�����AD����ԭ����֪��ȡ��AD�������ݲ�����Ϊ��Ҳ�����ܴ���255
		if(RKEY==0) {TxBuf[5]=1;} else {TxBuf[5]=0;}
	  if(LKEY==0) {TxBuf[6]=1;} else {TxBuf[6]=0;}
		TxBuf[0]++;
	  nRF24L01_TxPacket(TxBuf);//��������
		os_wait(K_IVL,2,0); //��һ����ʱ�����ݷ������
	}
}

void IO_and_Init()
{
		P1M0=0x00;  //P1��Ϊ����ģʽ
		P1M1=0xFF;
		P0M0=0XFF;  //����I/O������Ϊ׼˫��������ģʽ
		P0M1=0X00;
		P2M0=0X00;
		P2M1=0X00;
		P3M0=0X00;
		P3M1=0X00;
		P4M0=0X00;
		P4M1=0X00;
		P5M0=0Xff;
		P5M1=0X00;
		LKEY=1;    //���߰������I/O�ڵ�ƽ������Ϊ�͵�ƽ����
		RKEY=1;
		init_NRF24L01();  //��ʼ������ģ��
    adc_init();       //��ʼ��AD���ģ��
	  ax=getADCResult(0)-128;  //��¼�ϵ�ʱҡ�˵�������Ϊ��λ��������Ϊҡ����λҪΪ128��256/2
	  Delay(10);
	  by=getADCResult(3)-128;
    Delay(10);
	  bx=getADCResult(2)-128;        
	  Delay(10);
}