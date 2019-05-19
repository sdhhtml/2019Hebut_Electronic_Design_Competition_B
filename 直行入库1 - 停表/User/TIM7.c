#include "TIM6.h" 
#include "TIM4.h" 
#include "TIM3.h"
#include "TIM2.h" 
#include "led.h" 
#include "oled.h"
#include "imu.h"
#include "bsp_usart.h"
#include "TIM7.h" 
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "ALIENTEK.h"
extern unsigned char motor_mode;
extern float pitch,roll,yaw; 	        //ŷ����
extern float yaww_offset;
extern float yaww;
unsigned short int TIM7_10us_Count=0;
unsigned short int TIM7_1ms=0;
unsigned short int TIM7_500ms=0;
unsigned short int TIM7_1s=0;
unsigned short int TIM7_1min=0;
unsigned int TIM7_100ms=0;
unsigned char JS=1;//û�� 1 ��Ӳ�� 0
unsigned char JS_count=0;
unsigned char JS_Flag=0;
unsigned char CD=0;//�̶� ǰ��Ȧ0 ���Ȧ1
//unsigned char HBD=0;//�̶� ǰ��Ȧ0 ���Ȧ1
//unsigned char CD=1;//�̶� ǰ��Ȧ0 ���Ȧ1
unsigned char HBD=0;//�̶� ǰ��Ȧ0 ���Ȧ1
unsigned short int CD_T=0;
unsigned int Count_ms=0;//ÿһ�׶εļ�ʱ
unsigned char BH_flag=0;//�ջ�8���׶�
unsigned int Time_1=2500;
unsigned int Time_2=1250;
unsigned int Time_3=3000;
unsigned int Time_4=1800;
unsigned int Time_5=101;
unsigned int Time_6=101;
unsigned int Time_7=101;
unsigned int Time_8=101;

unsigned int Time_10=0;//�滻4 ��ֱ
unsigned int Time_11=0;//
unsigned int Time_12=0;

unsigned char ZX_flag=0;//0ʱ����ǰ�����Թ� 1ʱ�����м������Թ�
unsigned int YB_JL=0;//����
unsigned int TB_time=0;//ͣ��ʱ��
// �ж����ȼ�����
static void TIM7_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn ;	
		// ���������ȼ�Ϊ 1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
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


static void TIM7_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = 9;	// 999 100hz 4999 20hz  

	  // ʱ��Ԥ��Ƶ��Ϊ
    TIM_TimeBaseStructure.TIM_Prescaler= 71;//72M 72 000 000        1 000 00hz
	
		// ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
    //TIM_TimeBaseStructure.TIM_ClockDivision=0;
		
		// ����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// �ظ���������ֵ��������ʱ��û�У����ù�
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
		
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM7,TIM_IT_Update|TIM_IT_Trigger,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM7, ENABLE);	
		
}

void TIM7_Init(void)
{
	TIM7_NVIC_Config();
	TIM7_Mode_Config();
}
void TIM7_IRQHandler(void)//10ms
{
	if ( TIM_GetITStatus( TIM7, TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM7 , TIM_FLAG_Update);
						//-------------------------------------------hongwai
		TIM7_10us_Count++;
		TIM7_100ms++;
		if(TIM7_100ms>9999)//10us*10000 1000 100ms
		{
			TIM7_100ms=0;
		}
		if(TIM7_100ms>1&&TIM7_100ms<3)//����������
		{
			PBout(14)=1;
			PBout(15)=1;
		}
		else
		{
			PBout(14)=0;
			PBout(15)=0;
		}
		if(TIM7_10us_Count>99)//1ms
		{
			TIM7_10us_Count=0;
			TIM7_1ms++;
			if(PFin(7)==0)
			{
				JS=0;
				PEout(10)=0;
				PCout(11)=1;
				PCout(10)=0;
			}
			else//��Ӳ��
			{
				if(JS==0)
				{
					JS=1;
					JS_count+=1;
					if(JS_count==1)
					{
						YB_JL=(unsigned int)((float)(CaptureNumberB2*2));
					}
					if(JS_count==4)
					{
						CD=1;
						CD_T=TIM7_1s*1000+TIM7_500ms*500+TIM7_1ms;
					}
				}
				JS=1;
				PEout(10)=1;
				PCout(11)=0;
				PCout(10)=1;
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
			if(PFin(0)==0)
					{
						F0_In=0;
					}
					else
					{
						F0_In=1;
					}
			if(PFin(1)==0)
					{
						F1_In=0;
					}
					else
					{
						F1_In=1;
					}
			if(PFin(2)==0)
					{
						F2_In=0;
					}
					else
					{
						F2_In=1;
					}
			if(PFin(3)==0)
					{
						F3_In=0;
					}
					else
					{
						F3_In=1;
					}
			if(CD==0)
			{
				if(G5_In==0)
				{
					if(G4_In==0)
					{
						if(ZX_flag==1)
						{
								if((G3_In==0&&G2_In==0)||(G3_In==1&&G2_In==1))//ȫ�� ֱ��
								{
									Forword_M();
								}
								else if(G3_In==1&&G2_In==0)//������ ��Ҫ���1
								{
									Forword_L1();
								}
								else if(G3_In==0&&G2_In==1)//���ĺ� ��Ҫ�ҹ�1
								{
									Forword_R1();
								}
						}
						else if(ZX_flag==0)
						{
							if((F0_In==0&&F1_In==0)||(F0_In==1&&F1_In==1))//ȫ�� ֱ��
								{
									Forword_M();
								}
								else if(F0_In==1&&F1_In==0)//������ ��Ҫ���1
								{
									Forword_L1();
								}
								else if(F0_In==0&&F1_In==1)//���ĺ� ��Ҫ�ҹ�1
								{
									Forword_R1();
								}
							if(G3_In==1||G2_In==1)
							{
								ZX_flag=1;
							}
						}
					}
					else//��2���� ��Ҫ���2
					{
						Forword_L2();
					}
				}
				else//��1���� ��Ҫ���3
				{
					Forword_L3();
				}
			}
			else if(CD==1)
			{
				if(TIM7_1s*1000+TIM7_500ms*500+TIM7_1ms-CD_T>4999)
					{
						if(HBD==0)
						{
							if(ultrasonic3.distance>600)//�����ж�
							{
								Forword_M();
							}
							else if(ultrasonic3.distance!=0)
							{
								HBD=1;
								BH_flag=1;//��һ�׶�
							}
						}
						else if(HBD==1)//�̶�����
						{
							if(BH_flag==1)
							{
								Count_ms++;
								if(Count_ms<Time_1-100)
								{
									Forword_L3();
								}
								else if(Count_ms<=Time_1)
								{
									Stop_it();
								}
								else if(Count_ms>Time_1)
								{
									BH_flag=2;
									Count_ms=0;
								}
							}
							else if(BH_flag==2)
							{
								Count_ms++;
								if(Count_ms<Time_2-100)
								{
									Forword_M();
								}
								else if(Count_ms<=Time_2)
								{
									Stop_it();
								}
								else if(Count_ms>Time_2)
								{
									BH_flag=3;
									Count_ms=0;
								}
							}
							else if(BH_flag==3)
							{
								Count_ms++;
								if(Count_ms<Time_3-100)
								{
									Forword_R3();
								}
								else if(Count_ms<=Time_3)
								{
									Stop_it();
								}
								else if(Count_ms>Time_3)
								{
									BH_flag=4;
									Count_ms=0;
								}
							}
							else if(BH_flag==4)
							{
								Count_ms++;
								if(Count_ms<Time_4-100)
								{
									Forword_M();
								}
								else if(Count_ms<=Time_4)
								{
									//Stop_it();
								}
								else if(Count_ms>Time_4)
								{
									//BH_flag=5;
									Count_ms=0;
								}
								/////////////
								if(G4_In==1)
								{
									BH_flag=9;
									TB_time=TIM7_1s*1000+TIM7_500ms*500+TIM7_1ms;
								}
								///////////////////////////
								
							}
							else if(BH_flag==5)
							{
								Count_ms++;
								if(Count_ms<Time_5-100)
								{
									Forword_R3();
								}
								else if(Count_ms<=Time_5)
								{
									Stop_it();
								}
								else if(Count_ms>Time_5)
								{
									BH_flag=6;
									Count_ms=0;
								}
							}
							else if(BH_flag==6)
							{
								Count_ms++;
								if(Count_ms<Time_6-100)
								{
									Forword_M();
								}
								else if(Count_ms<=Time_6)
								{
									Stop_it();
								}
								else if(Count_ms>Time_6)
								{
									BH_flag=7;
									Count_ms=0;
								}
							}
							else if(BH_flag==7)
							{
								Count_ms++;
								if(Count_ms<Time_7-100)
								{
									Forword_L3();
								}
								else if(Count_ms<=Time_7)
								{
									Stop_it();
								}
								else if(Count_ms>Time_7)
								{
									BH_flag=8;
									Count_ms=0;
								}
							}
							else if(BH_flag==8)
							{
								Count_ms++;
								if(Count_ms<Time_8-100)
								{
									Forword_M();
								}
								else if(Count_ms<=Time_8)
								{
									Stop_it();
								}
								else if(Count_ms>Time_8)
								{
									BH_flag=9;
									Count_ms=0;
								}
							}
							else if(BH_flag==9)
							{
								Stop_it();
							}
						}
					}
				else
					{
						Stop_it();
						if(TIM7_500ms==0)
						{
							PEout(10)=0;
							PCout(11)=1;
							PCout(10)=0;
						}
						else
						{
							PEout(10)=1;
							PCout(11)=0;
							PCout(10)=1;
						}
					}
			}
		}
		if(TIM7_1ms>499)//500ms
		{
			TIM7_1ms=0;
			digitalToggle(GPIOE,GPIO_Pin_9);
			TIM7_500ms++;
		}
		if(TIM7_500ms>1)//1s
		{
			TIM7_500ms=0;
			TIM7_1s++;
		}
		if(TIM7_1s>59)//1min
		{
			TIM7_1min++;
		}
		if(TIM7_1min>59)
		{
			TIM7_1min=0;
		}			
	}		 	
}
/*********************************************END OF FILE**********************/
