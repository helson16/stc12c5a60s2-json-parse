#include <STC12C5A.h>

#include <intrins.h> //Keil library (is used for _nop()_ operation)  
#include <math.h>    //Keil library  
#include <stdio.h>   //Keil library
#include <string.h>
#include <common.h>

#define uint8 	unsigned char 
#define uint16 	unsigned int
#define uint32 	unsigned long
typedef uint8  u8;
typedef uint16 u16;
typedef uint32 u32;

extern  uint16  testRegister;	
extern  uint8	localAddr;
extern  uint8	sendCount;		
extern  uint8	receCount,rece2Count;	   
extern  uint8	getCount;	  

extern uint8  xdata sendBuf[40];
extern uint8  xdata receBuf[256];
extern uint8  xdata rece2Buf[32];
extern  bit RxFlag;		

void UART_T (unsigned char UART_data);

void UART_TC (unsigned char *str);


uint16 crc16(uint8 *puchMsg, uint16 usDataLen);
void UartSendstr(uint8 len,uint8 stemp[]);	 //void beginSend(void);
void Uart2Sendstr(uint8 len,uint8 stemp[]);
void checkUart2data(void);
void rep_Error(void);

void T485_uart2(uint8 T_num);

#define TX_buf_uart2_len 32
#define TX_buf_uart1_len 32
#define Send_485_len 32
u8 Modbus_485_uart2_TX(void);

void delay(uint8 x);
void delay_1s(void);
void port_init(void);
void timer0_init(void);
void Timer0_Init(void);
void Timer1_Init(void);
void uart1_init(void);
void uart2_init(void);

void delay1000ms(void);
void Delay_ms(unsigned int);

#define  DO1   	P21   
#define  RS485E  	P24 
#define  cai_Right	1 
#define  cai_Error	2 
#define  diRight    3
#define  coRight    4
#define  diError    5
#define  coError    6

void T485_uart2_Write(uint8 u);

void Single_Relay(u8 ch,u8 sw);
