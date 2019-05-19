#ifndef __STEER_H_
#define __STEER_H_
#include <stm32f10x.h>
void Observation_center(void);//观察中间
void Observe_the_right_side(void);//观察右边
void Color_reset(void);//观察复位

void Reading_Two_Dimensional_Code(void);//读取二维码
void Read_reset(void);//读取复位

void Grab_left(void);//抓取左边
void Drop_left(void);//放下左边
void Left_reduction(void);//左边复位

void Grasp_Center(void);//抓取中间
void Drop_middle(void);//放下中间
void Intermediate_reset(void);//中间复位

void Unfetched_state(void);//准备抓取状态

#endif

