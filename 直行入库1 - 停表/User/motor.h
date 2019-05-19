#ifndef __MOTOR_H
#define	__MOTOR_H
#include "stm32f10x.h"
void chumen(void);
void quzhaoerweima(void);

void fromtake_tohome(void);
void all_oled(void);
void order(void);//顺序


void red_123(void);//到达左取点123
void downred_123(void);//到达红色放置点123
void returnred_123(void);//回到左取点123

void green_123(void);//到达中间取点123
void downgreen_123(void);//到达绿色放置点123
void returngreen_123(void);//回到中间取点

void blue_123(void);//到达中间取点123
void downblue_123(void);//到达蓝色放置点123
void returnblue_123(void);//回到蓝色取点


void code_mid(void);//从二维码到中间
void mid_place(void);//从中间到放置
void place_mid(void);//从放置到中间
void place_start(void);//从放置回初始点。
void mid_place_bian(void);//从中间到放置bian


void new_chumen(void);//激光出门测试1
void new_qrcode(void);//红外对管去找二维码
void new_qrget(void);//从二维码到中间抓取区
void new_placemid(void);//从抓取到中间放置点
void new_placeedge(void);//从抓取到边缘放置点
void new_getmid(void);//从放置点到中间抓取点
void gohome(void);
#define y_speed 260
#endif
/****/
