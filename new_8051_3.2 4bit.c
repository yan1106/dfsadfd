#include <REG320.h>
#include <intrins.h>

void Keypad(void);
void ShowSeg();
void SettingIO(void);
void delay(unsigned int time);
void setrecord();
unsigned char numbers[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0f};                          
//unsigned char seg_num[]={0xe0,0xd0,0xb0,0x70};
unsigned char seg_num[]={0xf0,0xf0,0xf0,0xf0};
unsigned char digit0 = 10,digit1 = 10,digit2 = 10,digit3 = 10;
int num_show[4]={0,0,0,0},count=0,i=0,a,b,recod,cnt=0;
int add = 0;
void chang(void)
{
	num_show[3]=num_show[2];
	num_show[2]=num_show[1];
	num_show[1]=num_show[0];
	count++;
	cnt++;
}

void clear(void)
{
	for(i=0;i<4;i++)
	{
			seg_num[i]=0xf0;
			num_show[i]=0;
	}
	count=0;
	cnt=0;
}

void Keypad(void) interrupt 8
{
	P1=0xef; 
	_nop_();	
	if(P1==0xee){		//1
		chang();
		while(P1==0xee);
		num_show[0]=1;
	}
	else if(P1==0xed){
		chang();
		while(P1==0xed);
		num_show[0]=2;
		
	}	
	else if(P1==0xeb){
		chang();
		while(P1==0xeb);
		num_show[0]=3;
	
	}	
	else if(P1==0xe7){//a//+
		while(P1==0xe7);
		a=(num_show[3]*1000)+(num_show[2]*100)+(num_show[1]*10)+(num_show[0]);
		recod=1;
		clear();
	}	

	P1=0xdf;
	_nop_();		
	if(P1==0xde){
		chang();
		while(P1==0xde);
		num_show[0]=4;
		
	}
	else if(P1==0xdd){
		chang();
		while(P1==0xdd);
		num_show[0]=5;
		
	}	
	else if(P1==0xdb){
		chang();
		while(P1==0xdb);
		num_show[0]=6;
		
	}	
	else if(P1==0xd7){//b//-
		while(P1==0xd7);
		a=(num_show[3]*1000)+(num_show[2]*100)+(num_show[1]*10)+(num_show[0]);
		recod=2;
		clear();
	}	
	
	P1=0xbf;
	_nop_();		
	if(P1==0xbe){
		chang();
		while(P1==0xbe);
		num_show[0]=7;
		
	}
	else if(P1==0xbd){
		chang();
		while(P1==0xbd);
		num_show[0]=8;
		
	}	
	else if(P1==0xbb){
		chang();
		while(P1==0xbb);
		num_show[0]=9;
		
	}	
	else if(P1==0xb7){//c *
		while(P1==0xb7);
		a=(num_show[3]*1000)+(num_show[2]*100)+(num_show[1]*10)+(num_show[0]);
		recod=3;
		clear();				
		
	}	
	
	P1=0x7f; 
	_nop_();		
	if(P1==0x7e){//* clear
		clear();
		while(P1==0xb7e);
	}
	else if(P1==0x7d){//0	
		chang();
		while(P1==0x7d);
		num_show[0]=0;	
	}	
	else if(P1==0x7b){//#
		while(P1==0x7b);
		
		if(recod==1)
		{
			b=(num_show[3]*1000)+(num_show[2]*100)+(num_show[1]*10)+(num_show[0]);
			a=a+b;
			
			setrecord();
			
			num_show[0]=a%10;
			num_show[1]=(a/10)%10;
			num_show[2]=(a/100)%10;
			num_show[3]=(a/1000)%10;
			a=0;
			b=0;
		}
		else if(recod==2){
			b=(num_show[3]*1000)+(num_show[2]*100)+(num_show[1]*10)+(num_show[0]);
			a=a-b;
			if(a<0)
			{
				a=a+10000;
			}
			setrecord();
			
			
			num_show[0]=a%10;
			num_show[1]=(a/10)%10;
			num_show[2]=(a/100)%10;
			num_show[3]=(a/1000)%10;
			a=0;
			b=0;
		}
		else if(recod==3){
			b=(num_show[3]*1000)+(num_show[2]*100)+(num_show[1]*10)+(num_show[0]);
			a=a*b;
			
			setrecord();
			
			num_show[0]=a%10;
			num_show[1]=(a/10)%10;
			num_show[2]=(a/100)%10;
			num_show[3]=(a/1000)%10;
			a=0;
			b=0;
		}
		else if(recod==4){
			b=(num_show[3]*1000)+(num_show[2]*100)+(num_show[1]*10)+(num_show[0]);
			a=a/b;
			
			setrecord();
			
			num_show[0]=a%10;
			num_show[1]=(a/10)%10;
			num_show[2]=(a/100)%10;
			num_show[3]=(a/1000)%10;
			a=0;
			b=0;
		}

	}	
	else if(P1==0x77){//D /
		while(P1==0x77);
		a=(num_show[3]*1000)+(num_show[2]*100)+(num_show[1]*10)+(num_show[0]);
		recod=4;
		clear();
	
	}
	if(count == 5)
	{
		clear();
	}
	//initiate
	P1 = 0x0f;
	INTFLG  = 0x00;
}
void setrecord(){
	
	if( a < 10)
			cnt = 1;
	else if(a < 100)
			cnt = 2;
	else if(a < 1000)
			cnt = 3;
	else if(a < 10000)
			cnt = 4;

	count=0;
}


void setshow(){
	if(cnt == 0)
	{
		for(i=0;i<4;i++)
		{
			seg_num[i]=0xf0;
			
		}
	}
	if(cnt == 1)
		seg_num[0]=0xe0;
		seg_num[1]=0xf0;
		seg_num[2]=0xf0;
		seg_num[3]=0xf0;
	if(cnt == 2){
		seg_num[0]=0xe0;
		seg_num[1]=0xd0;
		seg_num[2]=0xf0;
		seg_num[3]=0xf0;
	}

	if(cnt == 3){
		seg_num[0]=0xe0;
		seg_num[1]=0xd0;
		seg_num[2]=0xb0;
		seg_num[3]=0xf0;
	}
		
	if(cnt == 4){
		seg_num[0]=0xe0;
		seg_num[1]=0xd0;
		seg_num[2]=0xb0;
		seg_num[3]=0x70;
	}
}
void ShowSeg()
{
	unsigned char segIndex = 0;
	segIndex = add++ % 4;
	
	setshow();		
	P0 = seg_num[segIndex] + numbers[num_show[segIndex]];
	if(add==4)
	{
		add=0;
	}
}

void delay(unsigned int time)
{
   	unsigned int n;
	{
		n=5; 
		while(n>0) 
			n--;
		time--;
	}
}


void SettingIO(void)		//interrupt
{
	p0oe = 0xff;
	P0 = 0xff;
	p2oe = 0xff;
	P1MODL = 0x00;			//PORT1低四位設為Open Drain (others pin) with internal pull up resistor	
	P1MODH = 0xaa;			//PORT1高四位設為CMOS output	
	P1 = 0x0f;				//PORT1設為0fh以用至偵測是否按下任一按鍵
	IE = 0x80;				//Global Interrupt Enable
	INTE1 = 0x02; 			//PORT1 PIN CHANGE Interrupt ENABLE
	IP1H  = 0x02;			//PORT1 PIN CHANGE Interrupt優先權設為1表示於所有中斷中 優先權較高
	P1WKUP  = 0x0f;		//PORT1 低四位之PIN設為中斷觸發
	INTFLG  = 0x00;		//PORT1 PIN CHANGE INT 旗標初始值設為零
}



void main()
{
	SettingIO();
	while(1)
	{	
		ShowSeg();
	}
}
