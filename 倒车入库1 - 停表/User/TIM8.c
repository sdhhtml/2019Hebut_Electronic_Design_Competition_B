#include "TIM8.h"
#include "TIM1.h"
#include "led.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
extern u16 tim8ch1;
extern u16 tim8ch2;
static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输出比较通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
static void ADVANCE_TIM_Mode_Config(void)
{
  // 开启定时器时钟,即内部时钟CK_INT=72M
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=999;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= 239;	//144mhz
	//1000 000
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
  TIM_ClearITPendingBit(TIM8,TIM_IT_Update);
	TIM_UpdateRequestConfig(TIM8,TIM_UpdateSource_Regular); //设置只有计数溢出作为更新中断
	/*--------------------输出比较结构体初始化-------------------*/		
	
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 互补输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable; 
	// 设置占空比大小
	TIM_OCInitStructure.TIM_Pulse = 350;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// 输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// 互补输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

	TIM_OC1Init(TIM8, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 655;
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 475;
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	
	// 使能计数器
	TIM_Cmd(TIM8, ENABLE);	
	// 主输出使能，当使用的是通用定时器时，这句不需要
	TIM_CtrlPWMOutputs(TIM8, ENABLE);

}
void TIM8_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}
void TIM8_Init(void)
{
ADVANCE_TIM_GPIO_Config();	
	ADVANCE_TIM_Mode_Config();
	//TIM8_NVIC_Config();
}
/*
3号舵机 TIM8CH1-C6  990收 240放
4号舵机 TIM8CH2-C7  700夹死 500放
*/
void TIM8Control(void)
{
	if(status.TIM8CH1_up==1)//q
	{
		tim8ch1+=10;if(tim8ch1>=990)tim8ch1=990;
		TIM_SetCompare1(TIM8,tim8ch1);	
		printf("tim8ch1=%d\r\n",tim8ch1);
		status.TIM8CH1_up=0;
	}
	if(status.TIM8CH1_down==1)//w
	{
		tim8ch1-=10;if(tim8ch1<=160)tim8ch1=160;
		TIM_SetCompare1(TIM8,tim8ch1);		
		printf("tim8ch1=%d\r\n",tim8ch1);
		status.TIM8CH1_down=0;
	}	
	
	
	if(status.TIM8CH2_up==1)//e
	{
		tim8ch2+=10;if(tim8ch2>=990)tim8ch2=990;
		TIM_SetCompare2(TIM8,tim8ch2);		
		printf("tim8ch2=%d\r\n",tim8ch2);
		status.TIM8CH2_up=0;
	}
	if(status.TIM8CH2_down==1)//r
	{
		tim8ch2-=10;if(tim8ch2<=300)tim8ch2=300;
		TIM_SetCompare2(TIM8,tim8ch2);		
		printf("tim8ch2=%d\r\n",tim8ch2);		
		status.TIM8CH2_down=0;
	}
}
void TIM8_test(void)
{
	int i=0;
	for(i=280;i<420;i+=20)
	{
		TIM_SetCompare1(TIM8,i);
		
		printf("%d\r\n",i);
	//	TIM_SetCompare2(TIM8,i);
//		TIM_SetCompare3(TIM8,i);
		Delay_ms(440);
		
	}	
	for(;i>280;i-=20)
	{
		TIM_SetCompare1(TIM8,i);
	//	TIM_SetCompare2(TIM8,i);
//		TIM_SetCompare3(TIM8,i);
		printf("%d\r\n",i);
		Delay_ms(200);
				
	}
	
}
/**/
