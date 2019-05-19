#ifndef __NEW_STRUCT_H_
#define __NEW_STRUCT_H_
#include <stm32f10x.h>
void Grasp_left(void);//(抓取左边二维码方向）
void Grasp_mid(void);//(抓取中间）
void Grasp_right(void);//(抓取右边发车区方向）
void Place_left(void);//放置左边二维码方向
void Place_mid(void);//放置中间
void Place_right(void);//放置右边发车区方向

void DJ_reset(void);

void Observing_color(void);//观察状态

void sehuan_fa(void);//观察发车区色环
void sehuan_zhong(void);//观察中间色环
void Observing_qrcode(void);//改变角度看二维码

#endif

