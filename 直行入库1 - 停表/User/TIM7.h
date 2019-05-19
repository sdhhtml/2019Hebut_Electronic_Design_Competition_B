#ifndef __TIM7_H
#define __TIM7_H


#include "stm32f10x.h"

void TIM7_Init(void);
extern unsigned short int TIM7_1ms;
extern unsigned short int TIM7_500ms;
extern unsigned short int TIM7_1s;
extern unsigned short int TIM7_1min;
extern unsigned char JS;
extern unsigned char JS_count;
extern unsigned char JS_Flag;
extern unsigned char CD;//程度 前半圈0 后半圈1
extern unsigned short int CD_T;//记录第四次触碰到硬币的时间
extern unsigned int Count_ms;//每一阶段的计时
extern unsigned char BH_flag;//闭环

extern unsigned int Time_1;//左
extern unsigned int Time_2;//直
extern unsigned int Time_3;//右
extern unsigned int Time_4;//直
extern unsigned int Time_5;//右
extern unsigned int Time_6;//直
extern unsigned int Time_7;//左
extern unsigned int Time_8;//直
extern unsigned int YB_JL;
extern unsigned int TB_time;
//unsigned char JS=1;//没有 1 有硬币 0
//unsigned char JS_count=0;
//unsigned char JS_Flag=0;
#endif
