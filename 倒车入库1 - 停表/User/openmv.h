#ifndef __OPENMV_H
#define	__OPENMV_H
#include "stm32f10x.h"
#include "led.h"
void Openmv_Init(void);
void OpenMv_00(void);//二维码
void OpenMv_11(void);//无
void OpenMv_01(void);//圆环单色
void OpenMv_10(void);//三色
void getwk(int ch);
void placewk(int ch);
void nine_mode(int ch);
void huizong(void);
#endif
