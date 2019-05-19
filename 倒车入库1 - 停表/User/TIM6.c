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
extern float pitch,roll,yaw; 	        //ŷ����
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
// �ж����ȼ�����
static void TIM6_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn ;	
		// ���������ȼ�Ϊ 1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            ����
 *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
 *  TIM_Period               ����
 *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */


static void TIM6_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=36M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period = 999;	// 999 100hz 4999 20hz  

	  // ʱ��Ԥ��Ƶ��Ϊ
    TIM_TimeBaseStructure.TIM_Prescaler= 71;//72M 72 000 000        1 000 00hz
	
		// ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=0;
		
		// ����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��������ʱ��û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
		
		// ʹ�ܼ�����
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
	if(F0_In==0)//��ǰ
	{
		OLED_ShowString(byte(9),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(9),line2,"1",6);	
	}
	if(F1_In==0)//��ǰ
	{
		OLED_ShowString(byte(11),line2,"0",6);	
	}
	else
	{
		OLED_ShowString(byte(11),line2,"1",6);	
	}
	if(F2_In==0)//����
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
