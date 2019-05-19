#include "led.h"
struct GLOBAL_FLAGS status = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
struct HONGWAI_FLAGS hongwaiflags = {0,0,0,0,0,0,0,0,0,0,0,0};
void led_init(void)
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		PCout(11)=0;
		PCout(10)=1;
		/* 关闭所有led灯	*/

		GPIO_SetBits(GPIOE, GPIO_Pin_8);
		GPIO_SetBits(GPIOE, GPIO_Pin_9);
		GPIO_SetBits(GPIOE, GPIO_Pin_10);
}


/**/

