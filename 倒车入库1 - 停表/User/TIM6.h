#ifndef __TIM6_H
#define __TIM6_H


#include "stm32f10x.h"

void TIM6_Init(void);
extern unsigned char G2_In;
extern unsigned char G3_In;
extern unsigned char G4_In;
extern unsigned char G5_In;

extern unsigned char F0_In;
extern unsigned char F1_In;
extern unsigned char F2_In;
extern unsigned char F3_In;

extern unsigned short int Flag_500ms;
void oled_hongwai(void);
#endif
