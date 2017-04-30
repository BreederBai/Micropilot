#ifndef __NRF24L01_H
#define __NRF24L01_H
typedef unsigned char uchar;
typedef unsigned char uint;	
sbit	CE	  =P4^6;
sbit	CSN		=P4^5;
sbit	SCK	  =P2^7;
sbit 	MOSI	=P2^6;
sbit 	MISO	=P2^5;
sbit	IRQ		=P2^4;
void Delay(unsigned int s);
void inerDelay_us(unsigned char n);
void init_NRF24L01(void);
void init_NRF24L012(void);
uint SPI_RW(uint uchar);
uchar SPI_Read(uchar reg);
void SetRX_Mode(void);
uint SPI_RW_Reg(uchar reg, uchar value);
uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars);
uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars);
void nRF24L01_RxPacket(unsigned char* rx_buf);
void nRF24L01_TxPacket(unsigned char * tx_buf);





#endif  