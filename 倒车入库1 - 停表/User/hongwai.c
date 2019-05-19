#include "hongwai.h"
#include "led.h"
#include "bsp_SysTick.h"
/*
红外对管	C4 C5 C9 A4 A5 A12
*/

void hongwai_init(void)
{
	GPIO_InitTypeDef GPIO_initStructure;
	//时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_7;
	GPIO_initStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_initStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_initStructure);
	
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOG,&GPIO_initStructure);
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB,&GPIO_initStructure);
}
/**/
