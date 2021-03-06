#ifndef __TIM4_H_
#define __TIM4_H_
#include <stm32f10x.h>
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
//定义PID结构体
typedef struct 
{
   __IO int      SetPoint;                                 //设定目标 Desired Value
   __IO int     SumError;//误差累计
	 __IO double   Proportion;                               //比例常数 Proportional Const
   __IO double   Integral;                                 //积分常数 Integral Const
   __IO double   Derivative;                               //微分常数 Derivative Const
	 __IO double      Error;                                //Error[-1]
   __IO int      LastError;                                //Error[-1]
	 __IO int      increment;
   __IO int      PrevError;                                //Error[-2]
	 __IO int      lastoutput;                                //Error[-2]
	 __IO int      output;
}PID;
void TIM4_Init(void);
void TIM4_SetPWM_Num(u16 value,u8 ch);
void IncPIDInit(void);
void pid_calculation1(PID *spid,int now);
void pid_calculation2(PID *spid,int now);
void pid_calculation3(PID *spid,int now);
void pid_calculation4(PID *spid,int now);
void forward(void);
void backward(void);
void leftward(void);
void rightward(void);
void Stop_it(void);
void correct_pid1(PID *spid);
void correct_pid2(PID *spid);
void correct_pid3(PID *spid);
void correct_pid4(PID *spid);
#define  P_DATA      2                                 //P参数
#define  I_DATA      0.7                               //I参数
#define  D_DATA      0.1                              //D参数
#define  setpoint      600 
#define  anglespeed      100
#define AngleError     1

#define MotorSpeed1 450
void forward1(void);
void backward1(void);
void leftward1(void);
void rightward1(void);
	int range_protect(int duty, int min, int max);//限幅保护
	
	
	void Forword_test(void);
	void Backword_test(void);
	void Forword_M(void);
	void Forword_L1(void);
	void Forword_L2(void);
	void Forword_L3(void);
	void Forword_R1(void);
		void Forword_R2(void);
			void Forword_R3(void);
#endif
/**/
