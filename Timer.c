#include <REG320.h>
#include<intrins.h>
void Timer0();
void Timer1();
void ShowSeg();
void clear1();
void clear0();
void SettingIO(void);
void delay(unsigned int time);

unsigned char numbers[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0xff};   //數字0-9,0xff表示都不亮                         
unsigned char seg_num[]={0xe0,0xd0,0xb0,0x70};										//控制七段顯示器哪顆亮
unsigned char digit0 = 0,digit1 = 0,digit2 = 0,digit3 = 0;							//用digit0-3代表存放七段顯示器要顯示的數值
//Timer副程式,interrupt 3 = vector 在0x1B的位置上
void Timer1() interrupt 3
{
	//重設計時站存器  TH1 TL1
	TH1 = 0xb1;
	TL1 = 0xe0;
	if(digit0 < 9){
		digit0 = digit0 + 1;
	}
	else{
		digit0 = 0;
		digit1 = digit1 + 1;
	}
	if(digit1 == 3 && digit0 ==	2){
		clear1();
	}
}
void clear1(){
	digit0 = 0;
	digit1 = 0;
}

void Timer0() interrupt 1
{
	//重設計時站存器  TH1 TL1
	TH0 = 0x63;
	TL0 = 0xc0;
	if(digit2 < 9){
		digit2 = digit2 + 1;
	}
	else{
		digit2 = 0;
		digit3 = digit3 + 1;
	}
	if(digit3 == 3 && digit2 ==	1){
		clear0();
	}
}

void clear0(){
	digit2 = 0;
	digit3 = 0;
}

//顯示數值在七段顯示器上
void ShowSeg() 
{
	unsigned char segIndex = 0;
	
	P0 = seg_num[segIndex] + numbers[digit0];
	delay(5);
	P0 = seg_num[segIndex+1] + numbers[digit1];
	delay(5);
	P0 = seg_num[segIndex+2] + numbers[digit2];
	delay(5);
	P0 = seg_num[segIndex+3] + numbers[digit3];
	delay(5);
}
//延遲副程式
void delay(unsigned int time)
{
  unsigned int n;
	while(time>0)
	{
		n=4; 
		while(n>0) 
			n--;
		time--;
	}
}
//設定會需要用到的IO
void SettingIO(void)
{
	CLKCON = 0x03; 		//預設值慢鐘80K
	IE = 0x08a;	  		//開Timer1 中斷
	TMOD = 0x11;	  	//Timer1 mode 1:16 Bits counter
	
	//Timer累加一次為2個system clk時間 =  1/40k 秒
	//故1秒 = Timer1累加40k次後溢位，又Timer1設為16Bits
	//65536-40000 = 25536 = 0x63C0
	//65536-20000 = 45536 = 0xb1e0
	
	TH1 = 0xb1;
	TL1 = 0xe0;

	TH0 = 0x63;
	TL0 = 0xc0;
	
	TR1 = 0x01; 		//Timer1 RUN

	TR0 = 0x01; 		//Timer0 RUN

	p0oe = 0xff;
	P0 = 0xff;
	P3MODL = 0x00; 		//設定P3.0~3為pull up input  ;20140123 debug
	P3MODH = 0xaa; 		//設定P3.4~7為CMOS output    ;20140123 debug
	P3 = 0xff;
}

void main()
{
	SettingIO();
	while(1){
		ShowSeg();
	}
	
}