#include "TIM4.h"
#include "bsp_usart.h"
#include "structconfig.h"
#include "led.h"
#include "oled.h"
unsigned int MotorSpeed=250;
PID sPID1;
static PID *sptr1 = &sPID1;
PID sPID2;
static PID *sptr2 = &sPID2;
PID sPID3;
static PID *sptr3 = &sPID3;
PID sPID4;
static PID *sptr4 = &sPID4;
extern unsigned char motor_mode;
extern double uart5_angle;
extern float yaww;
extern double angleoffset;
int quanju_x=0;
int quanju_y=0;
extern unsigned int CaptureNumberA1;//右
extern unsigned int CaptureNumberA2;//上
extern unsigned int CaptureNumberB1;
extern unsigned int CaptureNumberB2;
extern INT16_XYZ MPU9250_MAG_RAW;	     //地磁
unsigned char pp_data=4;
void IncPIDInit(void) 
{
    sptr1->LastError=0;            //Error[-1]
    sptr1->PrevError=0;            //Error[-2]
		sptr1->Error=0; 
    sptr1->Proportion=P_DATA;      //比例常数 Proportional Const
    sptr1->Integral=I_DATA;        //积分常数  Integral Const
    sptr1->Derivative=D_DATA;      //微分常数 Derivative Const
    sptr1->SetPoint=setpoint;           //设定目标Desired Value
		sptr1->lastoutput=0;
		sptr1->output=0;
		sptr1->increment=0;
		sptr1->SumError=0;
		
    sptr2->LastError=0;            //Error[-1]
    sptr2->PrevError=0;            //Error[-2]
		sptr2->Error=0; 
    sptr2->Proportion=P_DATA;      //比例常数 Proportional Const
    sptr2->Integral=I_DATA;        //积分常数  Integral Const
    sptr2->Derivative=D_DATA;      //微分常数 Derivative Const
    sptr2->SetPoint=setpoint;           //设定目标Desired Value
		sptr2->lastoutput=0;
		sptr2->output=0;
		sptr2->increment=0;
		sptr2->SumError=0;

    sptr3->LastError=0;            //Error[-1]
    sptr3->PrevError=0;            //Error[-2]
		sptr3->Error=0; 
    sptr3->Proportion=P_DATA;      //比例常数 Proportional Const
    sptr3->Integral=I_DATA;        //积分常数  Integral Const
    sptr3->Derivative=D_DATA;      //微分常数 Derivative Const
    sptr3->SetPoint=setpoint;           //设定目标Desired Value
		sptr3->lastoutput=0;
		sptr3->output=0;
		sptr3->increment=0;
		sptr3->SumError=0;

    sptr4->LastError=0;            //Error[-1]
    sptr4->PrevError=0;            //Error[-2]
		sptr4->Error=0; 
    sptr4->Proportion=P_DATA;      //比例常数 Proportional Const
    sptr4->Integral=I_DATA;        //积分常数  Integral Const
    sptr4->Derivative=D_DATA;      //微分常数 Derivative Const
    sptr4->SetPoint=setpoint;           //设定目标Desired Value
		sptr4->lastoutput=0;
		sptr4->output=0;
		sptr4->increment=0;
		sptr4->SumError=0;
	//	OLED_ShowNum(byte(6),line7,P_DATA,2,6);
}
void pid_calculation1(PID *spid,int now)
{
	spid->Error=spid->SetPoint-now;
	spid->SumError+=spid->Error;
	if(spid->SumError>=1300)spid->SumError=1300;
	if(spid->SumError<=0)spid->SumError=0;
	spid->output=spid->Proportion*spid->Error
	+spid->Integral*spid->SumError
	+spid->Derivative*(spid->Error-spid->LastError);
	spid->LastError=spid->Error;
	if(spid->output>200)spid->output=200;
	if(spid->output<200)spid->output=200;

	TIM4_SetPWM_Num(spid->output,1);
	TIM4_SetPWM_Num(spid->output,4);//
}
void pid_calculation2(PID *spid,int now)
{
	spid->Error=spid->SetPoint-now;
	spid->SumError+=spid->Error;
	if(spid->SumError>=1300)spid->SumError=1300;
	if(spid->SumError<=0)spid->SumError=0;	
	spid->output=spid->Proportion*spid->Error
	+spid->Integral*spid->SumError
	+spid->Derivative*(spid->Error-spid->LastError);
	spid->LastError=spid->Error;
	if(spid->output>200)spid->output=200;
	if(spid->output<200)spid->output=200;
	TIM4_SetPWM_Num(spid->output,2);
	TIM4_SetPWM_Num(spid->output,3);
}
void pid_calculation3(PID *spid,int now)
{
	spid->Error=spid->SetPoint-now;
	spid->SumError+=spid->Error;
	if(spid->SumError>=1300)spid->SumError=1300;
	if(spid->SumError<=0)spid->SumError=0;	
	spid->output=spid->Proportion*spid->Error
	+spid->Integral*spid->SumError
	+spid->Derivative*(spid->Error-spid->LastError);
	spid->LastError=spid->Error;
	if(spid->output>700)spid->output=700;
	if(spid->output<0)spid->output=0;
	TIM4_SetPWM_Num(spid->output,3);
}
void pid_calculation4(PID *spid,int now)
{
	spid->Error=spid->SetPoint-now;
	spid->SumError+=spid->Error;
	if(spid->SumError>=1300)spid->SumError=1300;
	if(spid->SumError<=0)spid->SumError=0;	
	spid->output=spid->Proportion*spid->Error
	+spid->Integral*spid->SumError
	+spid->Derivative*(spid->Error-spid->LastError);
	spid->LastError=spid->Error;
	if(spid->output>700)spid->output=700;
	if(spid->output<0)spid->output=0;
	TIM4_SetPWM_Num(spid->output,4);
//	TIM4_SetPWM_Num(spid->output,1);
//	TIM4_SetPWM_Num(spid->output,1);
}
void TIM4_NVIC_Config(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;


 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器		 
}
void TIM4_Mode_Config(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	// 改变指定管脚的映射 GPIO_Remap_SWJ_Disable SWJ 完全禁用（JTAG+SW-DP）
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	// 改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO

	TIM_TimeBaseStructure.TIM_Period = 999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =3; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 

	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	// 互补输出禁能
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; 	
	// 设置占空比大小
//	TIM_OCInitStructure.TIM_Pulse = 30;


  TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	// 输出通道空闲电平极性配置
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	

  TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    //使能TIM1预装载寄存器
  TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    //使能TIM1预装载寄存器
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    //使能TIM1预装载寄存器		
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC1

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
//	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
    //使能TIM1预装载寄存器


  TIM_ARRPreloadConfig(TIM4, ENABLE);  
    //设置中断源，只有溢出时才中断
  TIM_UpdateRequestConfig(TIM4,TIM_UpdateSource_Regular);
	// 清除中断标志位
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    // 使能中断
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
	//100//211 1s
	//500//1190
	TIM_SetCompare1(TIM4,500);
	TIM_SetCompare2(TIM4,500);
	TIM_SetCompare3(TIM4,500);
	TIM_SetCompare4(TIM4,500);
	TIM_SetCompare1(TIM4,0);
	TIM_SetCompare2(TIM4,0);
	TIM_SetCompare3(TIM4,0);
	TIM_SetCompare4(TIM4,0);
}
void TIM4_Init(void)
{
	//TIM4_NVIC_Config();
	TIM4_Mode_Config();
}
void TIM4_SetPWM_Num(u16 value,u8 ch)
{
	if (ch==1)
	{
		TIM_SetCompare1(TIM4,value);
	}
	if (ch==2)
	{
		TIM_SetCompare2(TIM4,value);
	}
	if (ch==3)
	{
		TIM_SetCompare3(TIM4,value);
	}
	if (ch==4)
	{
		TIM_SetCompare4(TIM4,value);
	}
}
void Stop_it(void)
{	
	TIM4_SetPWM_Num(0,1);
	TIM4_SetPWM_Num(0,2);
	TIM4_SetPWM_Num(0,3);
	TIM4_SetPWM_Num(0,4);
//	if(status.uart5==1)
//	status.uart5=0;
}
void forward(void)
{	
			PGout(13)=1;//1前
			PGout(14)=0;//0前
			PDout(8)=0;//0前
			PDout(9)=1;//1前
			TIM_SetCompare1(TIM4,MotorSpeed);//右轮
			TIM_SetCompare3(TIM4,MotorSpeed);//左轮
			correct_pid2(&sPID2);
			correct_pid4(&sPID4);
}
void backward(void)
{	

			PGout(13)=0;//1前
			PGout(14)=1;//0前
			PDout(8)=1;//0前
			PDout(9)=0;//1前
			TIM_SetCompare1(TIM4,MotorSpeed);//右轮
			TIM_SetCompare3(TIM4,MotorSpeed);//左轮
			correct_pid2(&sPID2);
			correct_pid4(&sPID4);	
}
void leftward(void)
{	
			PGout(11)=1;//1左
			PGout(12)=0;//0左
			PDout(10)=1;//1左
			PDout(11)=0;//0左
			TIM_SetCompare2(TIM4,MotorSpeed);//下轮
			TIM_SetCompare4(TIM4,MotorSpeed);//上轮	
			correct_pid1(&sPID1);
			correct_pid3(&sPID3);


}
void rightward(void)
{	
			PGout(11)=0;//1左
			PGout(12)=1;//0左
			PDout(10)=0;//1左
			PDout(11)=1;//0左
			TIM_SetCompare2(TIM4,MotorSpeed);//下轮
			TIM_SetCompare4(TIM4,MotorSpeed);//上轮	
			correct_pid1(&sPID1);
			correct_pid3(&sPID3);
}

void correct_pid1(PID *spid)
	//偏差顺时针正数，逆时针负数
	//顺时针需要
	//左轮后转  右轮前转 前轮左转  后轮右转
{
	spid->Error=yaww;
	if(spid->Error>1)
	{
			PGout(13)=0;//1前
			PGout(14)=1;//0前
		spid->output=anglespeed*spid->Error*spid->Error;
		spid->output=range_protect(spid->output,0,130);//限幅保护
	}
	else if(spid->Error<-1)
	{
			PGout(13)=1;//1前
			PGout(14)=0;//0前
		spid->output=anglespeed*spid->Error*spid->Error;
		spid->output=range_protect(spid->output,0,130);//限幅保护
	}	
	else
		spid->output=0;

	TIM_SetCompare1(TIM4,spid->output);
}
void correct_pid4(PID *spid)
	//偏差顺时针正数，逆时针负数
	//顺时针需要
	//左轮后转  右轮前转
{
	spid->Error=yaww;
	if(spid->Error>AngleError)
	{
			PDout(10)=0;//1左
			PDout(11)=1;//0左
		spid->output=anglespeed*spid->Error*spid->Error;
		spid->output=range_protect(spid->output,0,130);//限幅保护

	}
	else if(spid->Error<-AngleError)
	{
			PDout(10)=1;//1左
			PDout(11)=0;//0左	
		spid->output=anglespeed*spid->Error*spid->Error;
		spid->output=range_protect(spid->output,0,130);//限幅保护

	}	
	else
		spid->output=0;

	TIM_SetCompare4(TIM4,spid->output);
}
/*
//			PDout(3)=1;//1前
			PBout(3)=0;//1左
			PBout(4)=1;//0左
//			PBout(5)=0;//0前
//	TIM_SetCompare1(TIM4,200);//左轮
		TIM_SetCompare2(TIM4,200);//下轮
		TIM_SetCompare3(TIM4,200);//上轮
//	TIM_SetCompare4(TIM4,200);//右轮	
*/
void correct_pid2(PID *spid)//下轮
	//偏差顺时针正数，逆时针负数
	//顺时针需要
	//左轮后转  右轮前转yaww
	//前轮左转  后轮右转
{
	spid->Error=yaww;
	if(spid->Error>AngleError)
	{
			PGout(11)=1;//1左
			PGout(12)=0;//0左
		spid->output=anglespeed*spid->Error*spid->Error;
		spid->output=range_protect(spid->output,0,80);//限幅保护

	}
	else if(spid->Error<-AngleError)
	{
			PGout(11)=0;//1左
			PGout(12)=1;//0左
		spid->output=anglespeed*spid->Error*spid->Error;
		spid->output=range_protect(spid->output,0,80);//限幅保护
		//PGout(12)=0;

	}	
	else
		spid->output=0;

	TIM_SetCompare2(TIM4,spid->output);
}
void correct_pid3(PID *spid)//左轮
	//偏差顺时针正数，逆时针负数
	//顺时针需要
	//左轮后转  右轮前转
	//前轮左转  后轮右转
{
	spid->Error=yaww;
	if(spid->Error>1)
	{
			PDout(8)=0;//0前
			PDout(9)=1;//1前	
		spid->output=anglespeed*spid->Error*spid->Error;
		spid->output=range_protect(spid->output,0,80);//限幅保护

	}
	else if(spid->Error<-1)
	{
			PDout(8)=1;//0前
			PDout(9)=0;//1前	
		spid->output=anglespeed*spid->Error*spid->Error;
		spid->output=range_protect(spid->output,0,80);//限幅保护

	}	
	else
		spid->output=0;

	TIM_SetCompare3(TIM4,spid->output);
}

void forward1(void)
{	
			PGout(11)=1;//1前
			PGout(14)=0;//0前
	pid_calculation1(&sPID1,CaptureNumberA1);
//	TIM_SetCompare1(TIM4,MotorSpeed1);//左轮
//	TIM_SetCompare4(TIM4,MotorSpeed1);//右轮	
			correct_pid2(&sPID2);
			correct_pid3(&sPID3);
}
void backward1(void)
{	
			PGout(11)=0;//1前
			PGout(14)=1;//0前
//	TIM_SetCompare1(TIM4,MotorSpeed1);//左轮
//	TIM_SetCompare4(TIM4,MotorSpeed1);//右轮	
	pid_calculation1(&sPID1,CaptureNumberA1);
			correct_pid2(&sPID2);
			correct_pid3(&sPID3);
}
void leftward1(void)
{	
			PGout(12)=1;//1左
			PGout(13)=0;//0左
//	TIM_SetCompare2(TIM4,MotorSpeed1);//下轮
//	TIM_SetCompare3(TIM4,MotorSpeed1);//上轮
	pid_calculation2(&sPID2,CaptureNumberA2);
			correct_pid1(&sPID1);
			correct_pid4(&sPID4);
}
void rightward1(void)
{	
			PGout(12)=0;//1左
			PGout(13)=1;//0左
//	TIM_SetCompare2(TIM4,MotorSpeed1);//下轮
//	TIM_SetCompare3(TIM4,MotorSpeed1);//上轮
	pid_calculation2(&sPID2,CaptureNumberA2);
			correct_pid1(&sPID1);
			correct_pid4(&sPID4);
}
/******** 限幅保护 *********/
int range_protect(int duty, int min, int max)//限幅保护
{
	if (duty >= max)
	{
		return max;
	}
	if (duty <= min)
	{
		return min;
	}
	else
	{
		return duty;
	}
}
unsigned short int Speed_mid=200;
unsigned short int Offset1=30;
unsigned short int Offset2=65;
unsigned short int Offset3=120;
void Forword_test(void)
{
	PGout(13)=0;//1前
			PGout(14)=1;//0前
			PDout(8)=0;//0前
			PDout(9)=1;//1前
			PGout(11)=1;//1左
			PGout(12)=0;//0左
			PDout(10)=1;//1左
			PDout(11)=0;//0左
	TIM_SetCompare1(TIM4,200);
	TIM_SetCompare2(TIM4,200);
	TIM_SetCompare3(TIM4,200);
	TIM_SetCompare4(TIM4,200);
}
void Backword_test(void)
{
	PGout(13)=1;//1前
			PGout(14)=0;//0前
			PDout(8)=1;//0前
			PDout(9)=0;//1前
			PGout(11)=0;//1左
			PGout(12)=1;//0左
			PDout(10)=0;//1左
			PDout(11)=1;//0左
	TIM_SetCompare1(TIM4,200);
	TIM_SetCompare2(TIM4,200);
	TIM_SetCompare3(TIM4,200);
	TIM_SetCompare4(TIM4,200);
}
void Forword_M(void)
{
	PGout(13)=0;//1前
	PGout(14)=1;//0前
	PDout(8)=0;//0前
	PDout(9)=1;//1前
	PGout(11)=1;//1左
	PGout(12)=0;//0左
	PDout(10)=1;//1左
	PDout(11)=0;//0左
	TIM_SetCompare1(TIM4,Speed_mid);
	TIM_SetCompare2(TIM4,Speed_mid);
	TIM_SetCompare3(TIM4,Speed_mid);
	TIM_SetCompare4(TIM4,Speed_mid);
}
void Forword_L1(void)
{
	PGout(13)=0;//1前
	PGout(14)=1;//0前
	PDout(8)=0;//0前
	PDout(9)=1;//1前
	PGout(11)=1;//1左
	PGout(12)=0;//0左
	PDout(10)=1;//1左
	PDout(11)=0;//0左
	TIM_SetCompare1(TIM4,Speed_mid+Offset1);
	TIM_SetCompare2(TIM4,Speed_mid+Offset1);
	TIM_SetCompare3(TIM4,Speed_mid-Offset1);
	TIM_SetCompare4(TIM4,Speed_mid-Offset1);
}
void Forword_L2(void)
{
	PGout(13)=0;//1前
	PGout(14)=1;//0前
	PDout(8)=0;//0前
	PDout(9)=1;//1前
	PGout(11)=1;//1左
	PGout(12)=0;//0左
	PDout(10)=1;//1左
	PDout(11)=0;//0左
	TIM_SetCompare1(TIM4,Speed_mid+Offset2);
	TIM_SetCompare2(TIM4,Speed_mid+Offset2);
	TIM_SetCompare3(TIM4,Speed_mid-Offset2);
	TIM_SetCompare4(TIM4,Speed_mid-Offset2);
}
void Forword_L3(void)
{
	PGout(13)=0;//1前
	PGout(14)=1;//0前
	PDout(8)=0;//0前
	PDout(9)=1;//1前
	PGout(11)=1;//1左
	PGout(12)=0;//0左
	PDout(10)=1;//1左
	PDout(11)=0;//0左
	TIM_SetCompare1(TIM4,Speed_mid+Offset3);
	TIM_SetCompare2(TIM4,Speed_mid+Offset3);
	TIM_SetCompare3(TIM4,Speed_mid-Offset3);
	TIM_SetCompare4(TIM4,Speed_mid-Offset3);
}
void Forword_R1(void)
{
	PGout(13)=0;//1前
	PGout(14)=1;//0前
	PDout(8)=0;//0前
	PDout(9)=1;//1前
	PGout(11)=1;//1左
	PGout(12)=0;//0左
	PDout(10)=1;//1左
	PDout(11)=0;//0左
	TIM_SetCompare1(TIM4,Speed_mid-Offset1);
	TIM_SetCompare2(TIM4,Speed_mid-Offset1);
	TIM_SetCompare3(TIM4,Speed_mid+Offset1);
	TIM_SetCompare4(TIM4,Speed_mid+Offset1);
}
void Forword_R2(void)
{
	PGout(13)=0;//1前
	PGout(14)=1;//0前
	PDout(8)=0;//0前
	PDout(9)=1;//1前
	PGout(11)=1;//1左
	PGout(12)=0;//0左
	PDout(10)=1;//1左
	PDout(11)=0;//0左
	TIM_SetCompare1(TIM4,Speed_mid-Offset2);
	TIM_SetCompare2(TIM4,Speed_mid-Offset2);
	TIM_SetCompare3(TIM4,Speed_mid+Offset2);
	TIM_SetCompare4(TIM4,Speed_mid+Offset2);
}
void Forword_R3(void)
{
	PGout(13)=0;//1前
	PGout(14)=1;//0前
	PDout(8)=0;//0前
	PDout(9)=1;//1前
	PGout(11)=1;//1左
	PGout(12)=0;//0左
	PDout(10)=1;//1左
	PDout(11)=0;//0左
	TIM_SetCompare1(TIM4,Speed_mid-Offset3);
	TIM_SetCompare2(TIM4,Speed_mid-Offset3);
	TIM_SetCompare3(TIM4,Speed_mid+Offset3);
	TIM_SetCompare4(TIM4,Speed_mid+Offset3);
}
/**/
