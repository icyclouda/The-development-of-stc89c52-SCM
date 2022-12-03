#include <reg52.h>//4.0
sbit M1=P0^0;
sbit M1_base=P0^1;
sbit M2=P2^0;
sbit M2_base=P2^1;

 char a=0,b=0;
char ZKBL=0,ZKBR=0;
void u( char ch)//反馈情况
{
	SBUF=ch;
	while(!TI);//waiting
	TI=0;
	
}
void dsq0 () interrupt 1
{
	
	TH0=0xfe;
	TL0=0x90;
	
	if(a>100){a=0;}
	if(ZKBL>=0){
		if(a>=ZKBL){M1=1;M1_base=1;}
		else{M1=0;M1_base=1;}}
	else{
		if(a>=-ZKBL){M1=1;M1_base=1;}
		else{M1=1;M1_base=0;}}
	
	a++;
	
	if(b>100){b=0;}
	if(ZKBR>=0){
		if(b>=ZKBR){M2=1;M2_base=1;}
		else{M2=0;M2_base=1;}}
	else{
		if(b>=-ZKBR){M2=1;M2_base=1;}
		else{M2=1;M2_base=0;}}
b++;
	


}
void CSH(void)
{
	
	EA=1;
  PCON &= 0x7F;		//将smod=0不翻倍
	SCON = 0x50;		//SM0=0;SM1=1;SM2=0;REN=1;TB8=RB8=TI=RI=0
	TMOD = 0x21;		//T0模式2\T1模式3
	TL1 = 0xFD;			// 波特率9600
	TH1 = 0xFD;			
	TL0 = 0x00;
	TH0 = 0xFF;
	ET1 = 0;			//定时中断开
	ET0 = 1;
	TR1 = 1;
    TR0 = 1; 	//定时器1启动
	ES = 1;		//串口中断开
	PS=1;
}

void main()				
{
	M2=1;
	M1=1;
	M1_base=1;
	M2_base=1;
  CSH();		
	while(1);
}
 
void Uart_Bluetooth() interrupt 4		
{
			 char transmit;
			while(!RI);//waiting、、
			RI=0;
	    transmit=SBUF;
			switch(transmit)
			{
				case '0':ZKBR=ZKBL=0;break;//stop
				case '1':ZKBR=87;ZKBL=100;break;//RUSH!（老大）
				case '2':ZKBR=100;ZKBL=0;break;//大左转
				case '3':ZKBR=0;ZKBL=100;break;//大右转
				case '4':ZKBR=100;ZKBL=60;break;//快左转
				case '5':ZKBR=60;ZKBL=100;break;//快右转
				case '6':ZKBR=60;ZKBL=0;break;//小左转
				case '7':ZKBR=0;ZKBL=60;break;//小右转
				case '8':ZKBR=75;ZKBL=75;break;//中速（形同）
				case '9':ZKBR=55;ZKBL=55;break;//低速（虚设）
				case 'a':ZKBR=-69;ZKBL=-75;break;//直走倒车(左轮故障)
				default:break;
			}
		u(ZKBR);//检测值
		
}

