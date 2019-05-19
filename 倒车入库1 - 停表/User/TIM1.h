#ifndef __TIM1_H_
#define __TIM1_H_
#include <stm32f10x.h>


void TIM1_Init(void);
void TIM1Control(void);
void TIM1_test(void);

void get_left(void);//抓取左边
void put_down(void);//左边放下
void reset(void);//左边的复位

void dj_oled(void);

void get_mid(void);//抓取中间
void put_down2(void);//中间放下
void reset_s(void);//中间的复位

void order_getmid(void);//串口获取中间颜色
void order_getright(void);//串口获取右边颜色
void order_get(void);//串口获取颜色顺序
void get_code(void);//串口二维码信息

void tim_getmid(void);//舵机打角看中间
void tim_getright(void);//舵机打脚看右边
void tim_reset1(void);//舵机看颜色复位

void tim_getcode(void);//舵机打脚看二维码
void tim_reset2(void);//舵机看二维码复位

void Arbitrary_steering_gear(int wh ,int ch);
#define dj_delay 30
#endif
/*
void put_down(void)
{
for(tim1ch4=405;tim1ch4<450;tim1ch4+=20)	
	{
	TIM_SetCompare4(TIM1,tim1ch4);
	Delay_ms(100);
	}
	tim1ch4=450;
	TIM_SetCompare4(TIM1,tim1ch4);
for(tim1ch1=355;tim1ch1<660;tim1ch1+=20)	
	{
	TIM_SetCompare1(TIM1,tim1ch1);
	Delay_ms(100);
	}
	tim1ch1=660;
	TIM_SetCompare1(TIM1,tim1ch1);
for(tim8ch1=605;tim8ch1>545;tim8ch1-=20)
{	TIM_SetCompare1(TIM8,tim8ch1);
	Delay_ms(100);
}
	tim8ch1=545;
TIM_SetCompare1(TIM8,tim8ch1);	
for(tim1ch4=450;tim1ch4>360;tim1ch4-=20)	
	{
	TIM_SetCompare4(TIM1,tim1ch4);
	Delay_ms(100);
	}
	tim1ch4=360;
	TIM_SetCompare4(TIM1,tim1ch4);
for(tim8ch2=380;tim8ch2<475;tim8ch2+=20)
{	TIM_SetCompare2(TIM8,tim8ch2);
	Delay_ms(100);
}
	Delay_ms(100);
	tim8ch2=475;
TIM_SetCompare2(TIM8,tim8ch2);		
}
*/
