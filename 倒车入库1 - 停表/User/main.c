#include "stm32f10x.h"
#include "key.h"
#include "led.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "bsp_usart3.h"
#include "TIM2.h"
#include "TIM6.h"
#include "TIM3.h"
#include "TIM4.h"
#include "TIM1.h"
#include "TIM8.h"
#include "oled.h"
#include "filter.h"
#include "iic.h"
#include "mpu6050.h"
#include "imu.h"
#include "hongwai.h"
#include "uart4.h"
#include "mpu9250.h"
#include "DataScope_DP.h"
#include "1iic.h"
#include "HMC5983.h"
#include "uart5.h"
#include "encoder.h"
#include "motor.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "ALIENTEK.h"
#include "TIM7.h"
#include "steer.h"
#include "new_struct.h"
#include "openmv.h"
/*
��Դ����-�ⶨ
TIM1  CH1-A8 CH4-A11//���
TIM4	CH1-B6 CH2-B7 CH3-B8 CH4-B9//���
12345
A11 A8 C7 C6 C8
123(FAN)54

����			G13		G11			D8			D10
					G14		G12			D9			D11
					A2A3  A0A1   B0B1   A6A7
		
���� ������ B9  IN1 D10 D11 4
���� ������ B8  IN2 D8 D9 3
���� ������ B6		IN4 G13 G14 1
���� ������ B7		IN3 G11 G12 2

ADC 1 
0 A0
1 A1
6 A6
7 A7
8 B0
9 B1

TIM5	CH1-A0 CH2-A1//����
TIM6
TIM8	CH1-C6 CH2-C7 CH3-C8//���


LED	E8 E9 E10 
KEY	D3 D4 E7 D12 A12 F12 F13 F15

UART1 TX-A9�� RX-A10��

GND
A9 TX
A10 RX 
GND
5V
UART3 TX-B10�� RX-B11��
5V
GND
B11
B10
����Թ�	
 G5 G4 G3 G2 
 ������ A2A3
 
OLED B12 B13

�� �� ��
�ź� �� ��
�� �� ��
�ź� �� ��
	uart4
	tx C10
	rx C11
	mpu9250 D6 D7
	HMC5983 D4 D5
	uart5
	5v gnd D2 C12
	tx PC12 shang  
	rx PD2	xia
	F4 P2
	F5 P3
		IN4 IN5 GND 5V IN3 IN2 IN1
C6 C8 C7 A8 A11

B1
������C8 1��
��C6 1��
*/
u16 tim1ch1=1090;
u16 tim1ch4=1120;
u16 tim8ch1=360;//
u16 tim8ch2=655;//485
extern unsigned int MotorSpeed;
extern int quanju_x;
extern int quanju_y;
extern unsigned int CaptureNumberA1;//��
extern unsigned int CaptureNumberA2;//��
extern unsigned int CaptureNumberB1;
extern unsigned int CaptureNumberB2;
extern FLY_TYPE FLY;
extern FLOAT_ANGLE Att_Angle;  
extern unsigned char uart5_receice[8];
double uart5_angle=0;
double angleoffset=0;
extern unsigned char motor_mode;

float pitch,roll,yaw; 	        //ŷ����
float yaww_offset=0.0;
float yaww=0.0;

extern unsigned char uart5_mode;
extern unsigned char color_mid;
extern unsigned char color_right;

unsigned char flag_mid=0;
unsigned char flag_right=0;
unsigned char flag_code=0;
unsigned char code_count=0;
unsigned short int JG_distance=0;

extern unsigned char Openmv_Mode;//openmvģʽ
extern unsigned short int qd_code;//��ά��
extern unsigned char color_shunjian;//��ɫ˲��
extern unsigned char order_1;//���˳��
extern unsigned char order_2;//ɫ��˳��
extern unsigned short int color_mode;//ɫ��˳��

int main(void)
{
	OLED_Init();
	OLED_Clear();
	USART_Config();
	OLED_ShowString(byte(0),line1,"OLED_Init",6);	
	led_init();
	OLED_ShowString(byte(0),line1,"LED_Init",6);
	hongwai_init();
	OLED_ShowString(byte(0),line1,"hongwai__Init",6);
	TIM4_Init();//���
	OLED_ShowString(byte(0),line1,"TIM4_Init",6);
	Delay_ms(500);
	//TIM6_Init();//������ʱ��
	TIM2_Init();
	TIM3_Init();
	TIM7_Init();
	OLED_ShowString(byte(0),line1,"TIM6_Init",6);
	//GPIO_ResetBits(GPIOE, GPIO_Pin_8);
	//PEout(8)=0;
OLED_ShowString(byte(0),line1,"Start_Init",6);
	for(;;)
	{
		//Forword_test();
		//Backword_test();
		CSB3();
		CSB4();
		//printf("sss\r\n");
		oled_hongwai();
		//Delay_ms(100);
	}
	OLED_ShowString(byte(0),line1,"End",6);
	return 0;
	
}


/*
*/
/*********************************************END OF FILE**********************/
