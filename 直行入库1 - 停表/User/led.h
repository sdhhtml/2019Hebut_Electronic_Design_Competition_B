#ifndef __LED_H
#define	__LED_H
#include "stm32f10x.h"
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入
/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态
#define LED1_TOGGLE		 digitalToggle(GPIOB,GPIO_Pin_5)
#define LED2_TOGGLE		 digitalToggle(GPIOE,GPIO_Pin_5)
//#define LED3_TOGGLE		 digitalToggle(GPIOC,GPIO_Pin_2)
//#define LED4_TOGGLE		 digitalToggle(GPIOC,GPIO_Pin_3)
void led_init(void);
//系统状态
struct GLOBAL_FLAGS {
  //当步进电机正在运行时，值为1
  unsigned char flag1;
  //当串口接收到数据时，值为1
  unsigned char flag2;
  //当驱动器正常输出时,值为1
  unsigned char flag3;
	unsigned char TIM1CH1_up;
	unsigned char TIM1CH1_down;
	unsigned char TIM1CH4_up;
	unsigned char TIM1CH4_down;
	unsigned char TIM8CH1_up;
	unsigned char TIM8CH1_down;
	unsigned char TIM8CH2_up;
	unsigned char TIM8CH2_down;
	unsigned char MAG_flag;
	unsigned char uart5;
	unsigned char start;
//	unsigned char TIM8CH3;
};
struct HONGWAI_FLAGS {

  unsigned char x_0;
	unsigned char x_1;
  unsigned char x_2;
	unsigned char x_3;
	
	unsigned char y_0;
	unsigned char y_1;
	unsigned char y_2;
	unsigned char y_3;
	unsigned char y_4;
	unsigned char y_5;
	int x;
	int y;
};
extern struct GLOBAL_FLAGS status;
extern struct HONGWAI_FLAGS hongwaiflags;
#define TRUE 1
#define FALSE 0
#endif
/****/
