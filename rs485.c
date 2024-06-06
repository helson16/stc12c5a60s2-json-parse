#include  <main.h>

uint8  xdata rece2Buf[32]={0};
uint8	rece2Count;

void Uart2Sendstr(uint8 len,uint8 stemp[])   
{
	 
	uint8 i=0;
  // EA =0;
//	DO1=1;
	RS485E=1;
	for(i=0;i<len;i++)
 	 {

		S2CON &= ~S2TI;	
		S2BUF=*(stemp+i);
		while((S2CON & S2TI)==0x00);

//		busy=1;
    	S2CON &= ~S2TI; 
	
	}
	 RS485E=0;
//	 EA =1;
//	DO1=0;

}


/*********************************************************************/
void T485_uart2(uint8 u)
{
	uint8  Xphsoil_t[8]={0x00,0x03,0x00,0x00,0x00,0x02,0xC5,0xDA};//¶ÁÈ¡µØÖ·Îª0x00´«¸ÐÆ÷µÄ¼Ä´æÆ÷µØÖ·Îª0x000ºÍ0x0001µÄÁ½¸öÖµ
	uint16  crcData;
	Xphsoil_t[0] = 0x06;
	crcData = crc16(Xphsoil_t,6);
	Xphsoil_t[6] = crcData >> 8;
	Xphsoil_t[7] = crcData & 0xff;
	Uart2Sendstr(8,Xphsoil_t);

}
void T485_uart2_Write(uint8 u)
{
	uint8  Xphsoil_t[8]={0x10,0x06,0x02,0x00,0x00,0x05,0x4B,0x30};//¶ÁÈ¡µØÖ·Îª0x00´«¸ÐÆ÷µÄ¼Ä´æÆ÷µØÖ·Îª0x0200Öµ ´Ó»úµØÖ·Öµ
	uint16  crcData;
	Xphsoil_t[0] = 0x10;
	crcData = crc16(Xphsoil_t,6);
	Xphsoil_t[6] = crcData >> 8;
	Xphsoil_t[7] = crcData & 0xff;
	Uart2Sendstr(8,Xphsoil_t);
}										



