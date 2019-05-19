#include "TIM6.h" 
#include "TIM4.h" 
#include "TIM3.h" 
#include "TIM7.h" 
#include "led.h" 
#include "oled.h"
#include "imu.h"
#include "bsp_usart.h"
#include "TIM7.h" 
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "ALIENTEK.h"
unsigned char motor_mode;
extern float pitch,roll,yaw; 	        //欧拉角
extern float yaww_offset;
extern float yaww;
unsigned char G2_In=1;
unsigned char G3_In=1;
unsigned char G4_In=1;
unsigned char G5_In=1;
unsigned char F0_In=1;
unsigned char F1_In=1;
unsigned char F2_In=1;
unsigned char F3_In=1;
unsigned short int Flag_500ms=0;
// 中断优先级配置
static void TIM6_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn ;	
		// 设置主优先级为 1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            都有
 *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
 *  TIM_Period               都有
 *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */


static void TIM6_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=36M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period = 999;	// 999 100hz 4999 20hz  

	  // 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= 71;//72M 72 000 000        1 000 00hz
	
		// 时钟分频因子 ，基本定时器没有，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=0;
		
		// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM6, ENABLE);	
		
}

void TIM6_Init(void)
{
	TIM6_NVIC_Config();
	TIM6_Mode_Config();
}
/*
g2	you
g3	mid
g4 zuo
g5	xia
g6	shang

*/
void TIM6_IRQHandler(void)//1ms
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);
		Flag_500ms++;
		if(Flag_500ms>499)
		{
			Flag_500ms=0;
			digitalToggle(GPIOE,GPIO_Pin_9);
			PEout(10)=0;
		}
		if(PGin(2)==0)
		{
			G2_In=0;
		}
		else
		{
			G2_In=1;
		}
		if(PGin(3)==0)
		{
			G3_In=0;
		}
		else
		{
			G3_In=1;
		}
		if(PGin(4)==0)
		{
			G4_In=0;
		}
		else
		{
			G4_In=1;
		}
		if(PGin(5)==0)
		{
			G5_In=0;
		}
		else
		{
			G5_In=1;
		}
		oled_hongwai();
						//-------------------------------------------hongwai
	}

}
void oled_hongwai(void)
{
	if(G5_In==0)
	{
		OLED_ShowString(byte(0),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(0),line2,"1",6);	
	}
	if(G4_In==0)
	{
		OLED_ShowString(byte(2),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(2),line2,"1",6);	
	}
	if(G3_In==0)
	{
		OLED_ShowString(byte(4),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(4),line2,"1",6);	
	}
	if(G2_In==0)
	{
		OLED_ShowString(byte(6),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(6),line2,"1",6);	
	}
	if(F0_In==0)//左前
	{
		OLED_ShowString(byte(9),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(9),line2,"1",6);	
	}
	if(F1_In==0)//右前
	{
		OLED_ShowString(byte(11),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(11),line2,"1",6);	
	}
	if(F2_In==0)//后轮
	{
		OLED_ShowString(byte(13),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(13),line2,"1",6);	
	}
	if(F3_In==0)
	{
		OLED_ShowString(byte(15),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(15),line2,"1",6);	
	}
	if(JS==0)
	{
		OLED_ShowString(byte(8),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(8),line2,"1",6);	
	}
	OLED_ShowNum(byte(0),line3,TIM7_1min,3,6); 
	OLED_ShowString(byte(4),line3,"min",6);	
	OLED_ShowNum(byte(0),line4,TIM7_1s,3,6); 
	OLED_ShowString(byte(4),line4,"s",6);	
	OLED_ShowNum(byte(0),line5,TIM7_1ms+TIM7_500ms*500,4,6); 
	OLED_ShowString(byte(4),line5,"ms",6);	
	OLED_ShowNum(byte(0),line6,JS_count,4,6); 
	OLED_ShowString(byte(4),line6,"Count",6);	
	OLED_ShowNum(byte(0),line7,YB_JL,5,6); 
	OLED_ShowString(byte(8),line7,"mm",6);	
	OLED_ShowNum(byte(0),line8,TB_time,7,6); 
	OLED_ShowString(byte(10),line8,"ms",6);	
	
}

/*********************************************END OF FILE**********************/
