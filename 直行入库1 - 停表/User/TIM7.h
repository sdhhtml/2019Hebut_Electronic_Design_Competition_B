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
extern unsigned char CD;//�̶� ǰ��Ȧ0 ���Ȧ1
extern unsigned short int CD_T;//��¼���Ĵδ�����Ӳ�ҵ�ʱ��
extern unsigned int Count_ms;//ÿһ�׶εļ�ʱ
extern unsigned char BH_flag;//�ջ�

extern unsigned int Time_1;//��
extern unsigned int Time_2;//ֱ
extern unsigned int Time_3;//��
extern unsigned int Time_4;//ֱ
extern unsigned int Time_5;//��
extern unsigned int Time_6;//ֱ
extern unsigned int Time_7;//��
extern unsigned int Time_8;//ֱ
extern unsigned int YB_JL;
extern unsigned int TB_time;
//unsigned char JS=1;//û�� 1 ��Ӳ�� 0
//unsigned char JS_count=0;
//unsigned char JS_Flag=0;
#endif
