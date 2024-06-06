#include <main.h>
//bit ControlFlag=1;
void delay(uint8 x)
{
	uint8 y;
	for(;x>0;x--)
		for(y=5000;y>0;y--)
		{
			;
		} 	
} 
void delay_1s(void)   
{
    unsigned char a,b,c,n1;
    for(c=169;c>0;c--)
        for(b=228;b>0;b--)
            for(a=142;a>0;a--);
    for(n1=2;n1>0;n1--);
    _nop_;  //if Keil,require use intrins.h
}

void UART_T (unsigned char UART_data){ 
	ES=0;
	SBUF = UART_data;	
	while(TI == 0);		
	TI = 0;			
	ES=1;
}

void UART_TC (unsigned char *str){
	while(*str != '\0'){
		UART_T(*str++);
		//*str++;
	}
	*str = 0;
}
/*


void varInit(void)
{

}



void adc_init(void)
{

}

void watchdog_init(void)
{

}
*/




void port_init(void)
{
//P0=0xFF;
//	localAddr =~ P0;  
	P2=0xff; 
	P1=0xff;
	localAddr = 1;
	P32=0;
	P33=0;
	P34=0;
	P35=0;
	P36=0;
	P37=0;

	P4SW= P4SW|0x30;
}


void Timer0_Init(void)	
{
	AUXR &= 0x7F;			
	TMOD &= 0xF0;			
	TL0 = 0x00;				
	TH0 = 0xDC;				
	TF0 = 0;				
	TR0 = 1;			
	ET0 = 1;			
}

void Timer1_Init(void)		
{
	AUXR &= 0xBF;			
	TMOD &= 0x0F;			
	TL1 = 0x66;			
	TH1 = 0xFC;			
	TF1 = 0;				
	TR1 = 1;				
	ET1 = 1;			
}


void uart1_init(void)
{
	SCON=0x50;		
	TMOD=0x21;		
	PCON&=0x7f; 	
  TH1=0xFD;
	TL1=0xFD;	    
	TR1=1;			
	ES=1;          
	PS=1;        
	EA =1;          
}


void uart2_init(void)
{

	S2CON = 0x50;		
//	BRT = 0xe8;		    
	BRT = 0xfd;        
	AUXR =AUXR|0x10;		
	AUXR1=AUXR1|0x10;   
	EA=1;
	IE2=IE2|0x01;		

}




