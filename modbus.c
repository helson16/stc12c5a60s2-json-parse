#include <main.h>

uint16  testRegister;//=0x1111;	//ÓÃÓÚ²âÊÔ×ÖÖ·Ö·16£¬testRegisterÓ¦¸Ã¸³³õÖµ£¬³õÖµÈçºÎÉè¶¨£¿£¿Êµ¼Ê²ÉÊý¾ÝÊ±£¬µØÖ·ÈçºÎÓÃÄØ
uint8	sendCount;		//·¢ËÍ×Ö½Ú¸öÊý

//bit ByteEndFlag=0;	//×Ö½ÚÁ÷½áÊø±êÖ¾Î»
bit RxFlag=0;		//½ÓÊÕÍê×Ö½ÚÁ÷±êÖ¾Î»
bit RXAAOK=0;		//½ÓÊÕµ½Ö¡Í·±êÖ¾Î»
uint8	receCount=0;	    //½ÓÊÕµ½µÄ×Ö½Ú¸öÊý
uint8	localAddr;	//µ¥Æ¬»ú°åµÄµØÖ·,×¢Òâ¸Ä±äµØÖ·Ê±£¬Ð­ÒéµÄÐ£ÑéÎ»Ò²¸Ä±ä	
uint8  xdata sendBuf[40];
uint8  xdata receBuf[256]={0};

uint8  xdata storBuf[6]={0};

const uint8 code auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 
//CRCµÍÎ»×Ö½ÚÖµ±í
const uint8 code auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;


uint16 crc16(uint8 *puchMsg, uint16 usDataLen) 
{ 
	uint8 uchCRCHi = 0xFF ; // ¸ßCRC×Ö½Ú³õÊ¼»¯ 
	uint8 uchCRCLo = 0xFF ; // µÍCRC ×Ö½Ú³õÊ¼»¯  
	uint32 uIndex ; //CRCÑ­»·ÖÐµÄË÷Òý  
	while (usDataLen--) //´«ÊäÏûÏ¢»º³åÇø  
	{ 
		uIndex = uchCRCHi ^ *puchMsg++ ; //¼ÆËãCRC 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	return (uchCRCHi << 8 | uchCRCLo) ; 
}//uint16 crc16(uint8 *puchMsg, uint16 usDataLen)



 // ¿ªÊ¼·¢ËÍ,void beginSend(void)
void UartSendstr(uint8 len,uint8 stemp[])  
{
	uint8 i=0;
  // EA =0;
	for(i=0;i<len;i++)
 	 {
		TI=0;
		SBUF=stemp[i];
		while(TI==0);
		TI=0;
	 }
//	 EA =1;
}

void delay1000ms(void)   //Îó²î -0.000000000227us
{
    unsigned char a,b,c;
    for(c=13;c>0;c--)
        for(b=247;b>0;b--)
            for(a=142;a>0;a--);
    _nop_;  //if Keil,require use intrins.h
}

void delay500us(unsigned char a)
{
	unsigned char b;
    while(a--)
	{
		for(b=0;b<58;b++);
	}
}
void delay_1ms(void)   //Îó²î -0.018084490741us
{
    unsigned char a,b,c;
    for(c=8;c>0;c--)
        for(b=197;b>0;b--)
            for(a=2;a>0;a--);
}

void Delay_ms(unsigned int ch)
{
  unsigned int count;
	for(count=0;count<ch;count++)
	{
	  delay_1ms();
	}

}


//¶Á¼Ä´æÆ÷
uint8	TX_buf_usart1[TX_buf_uart1_len];//»º´æ´«¸ÐÆ÷Í¨¹ý´®¿Úuart2·¢¹ýÀ´µÄÊý¾Ý

void ControlRelay (void)
{
	
	uint8 tempAddr;
	uint16 tempData1;
	uint8  onOff;
	uint16 crcData;
	uint8 byteCount;
	uint8 i,n,j=0;
	n = 0;

 	for(j=0;j < getCount;j += 2)//¶ÁÈ¡¼ÌµçÆ÷±àºÅ¡¢¼ÌµçÆ÷ÃüÁî
	if(receBuf[j+6] == 0x11 || receBuf[j+6] == 0x22|| receBuf[j+6] == 0x33)
	{
		storBuf[n] = receBuf[j+5];
		storBuf[n+1] = receBuf[j+6];
		n += 2;
	}
	for(j=0;j < getCount;j += 2)
	{
		tempAddr = receBuf[j+5];//¼ÌµçÆ÷ºÅ
		onOff = receBuf[j+6];   //¼ÌµçÆ÷ÃüÁî
		
		if(onOff==0x11||onOff==0x22) //µçÆ½ÐÅºÅ¡¢Âö³åÐÅºÅ ¿ª¼ÌµçÆ÷
		{
			Single_Relay(tempAddr,1);					
		}
		else if(onOff==0x33)         //-------µçÆ½ÐÅºÅ ¹Ø¼ÌµçÆ÷
		{
			Single_Relay(tempAddr,0);
		
		}
				
	}
									//-------//Âö³åÐÅºÅ ¹Ø¼ÌµçÆ÷
	if(n!=0) 
 	{
 		delay1000ms();
 		delay1000ms();
 		delay1000ms();
 		delay1000ms();
 		for(i=0;i < n;i+=2)
 		{														  
 		    tempData1 = 0;														  
 			tempAddr = storBuf[i];
 			onOff = storBuf[i+1];           							                                    			
 			if(onOff == 0x11)		  //Âö³åÐÅºÅ ¹Ø¼ÌµçÆ÷
 			{         					     					   
 				Single_Relay(tempAddr,0);
 					                                                
 			}
 	     }
 	}
 	else
 		n=0;

	sendBuf[0] = 0xa5;
	sendBuf[1] = receBuf[1];//lora-net
	sendBuf[2] = receBuf[2];
	sendBuf[3] = 0x05;
	sendBuf[4] = receBuf[4];

    for(j=0;j < getCount;j += 2)
	{
		sendBuf[j+5] = receBuf[j+5];
		sendBuf[j+6] = receBuf[j+6];
	}
	if(receBuf[5]==1)
	{
	  sendBuf[6] = ~P13;
	}
	if(receBuf[5]==2)
	{
	  sendBuf[6] = ~P14;
	}

	byteCount = getCount + 5;

	crcData = crc16(sendBuf,byteCount);
	sendBuf[byteCount] = crcData >> 8;
	byteCount++;
	sendBuf[byteCount] = crcData & 0xff;
	sendBuf[byteCount+1] = 0xBF;
	sendCount = byteCount + 2;
	UartSendstr(sendCount,sendBuf);
	
	memset(receBuf,0,sizeof(receBuf));

}



