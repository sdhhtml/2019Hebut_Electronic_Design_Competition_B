#include "TIM2.h"
#include "bsp_usart.h"
uint32_t TIM_PscCLK = 1000000;
//TIM2
// 定时器输入捕获用户自定义变量结构体定义

TIM_ICUserValueTypeDef ultrasonic3 = {0,0,0,0,0,0};
TIM_ICUserValueTypeDef ultrasonic4 = {0,0,0,0,0,0};
// 中断优先级配置
static void TIM2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输入捕获通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;				 //LED0-->PB.5 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOE, &GPIO_InitStructure);					 
	 GPIO_ResetBits(GPIOB,GPIO_Pin_14);						 
	 GPIO_ResetBits(GPIOB,GPIO_Pin_15);						 					 		 

}
///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void TIM2_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
  // 开启定时器时钟,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

/*--------------------时基结构体初始化-------------------------*/	
  
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=0XFFFF;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= 71;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/*--------------------输入捕获结构体初始化-------------------*/	
	
	// 配置输入捕获的通道，需要根据具体的GPIO来配置
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	// 输入捕获信号的极性配置
	TIM_ICInitStructure.TIM_ICPolarity =TIM_ICPolarity_Rising;
	// 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// 定时器输入捕获初始化
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC3);	
  // 开启更新和捕获中断  
	TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC3, ENABLE );
	TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC4);	
  // 开启更新和捕获中断  
	TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC4, ENABLE );
	
	// 使能计数器
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_Init(void)
{
	TIM2_GPIO_Config();
	TIM2_NVIC_Config();
	TIM2_Mode_Config();		
}
void TIM2_IRQHandler(void)
{
	// 当要被捕获的信号的周期大于定时器的最长定时时，定时器就会溢出，产生更新中断
	// 这个时候我们需要把这个最长的定时周期加到捕获信号的时间里面去
	if ( TIM_GetITStatus ( TIM2, TIM_IT_Update) != RESET )               
	{	
		ultrasonic3.Capture_Period ++;
		ultrasonic4.Capture_Period ++;		
		TIM_ClearITPendingBit ( TIM2, TIM_FLAG_Update ); 		
	}


	// 上升沿捕获中断3
	if ( TIM_GetITStatus (TIM2, TIM_IT_CC3 ) != RESET)
	{
		// 第一次捕获
		if ( ultrasonic3.Capture_StartFlag == 0 )
		{
			// 计数器清0
			TIM_SetCounter ( TIM2, 0 );
			// 自动重装载寄存器更新标志清0
			ultrasonic3.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			ultrasonic3.Capture_CcrValue = 0;

			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			TIM_OC3PolarityConfig(TIM2, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			ultrasonic3.Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			ultrasonic3.Capture_CcrValue = 
			TIM_GetCapture3(TIM2);

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			TIM_OC3PolarityConfig(TIM2, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			ultrasonic3.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			ultrasonic3.Capture_FinishFlag = 1;		
		}

		TIM_ClearITPendingBit (TIM2,TIM_IT_CC3);	    
	}	
	// 上升沿捕获中断4
	if ( TIM_GetITStatus (TIM2, TIM_IT_CC4 ) != RESET)
	{
		// 第一次捕获
		if ( ultrasonic4.Capture_StartFlag == 0 )
		{
			// 计数器清0
			TIM_SetCounter ( TIM2, 0 );
			// 自动重装载寄存器更新标志清0
			ultrasonic4.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			ultrasonic4.Capture_CcrValue = 0;

			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			ultrasonic4.Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			ultrasonic4.Capture_CcrValue = 
			TIM_GetCapture4(TIM2);

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			ultrasonic4.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			ultrasonic4.Capture_FinishFlag = 1;		
		}

		TIM_ClearITPendingBit (TIM2,TIM_IT_CC4);	    
	}		
}
void CSB3(void)
{
						if(ultrasonic3.Capture_FinishFlag == 1)
							{
								// 计算高电平时间的计数器的值
								ultrasonic3.time = ultrasonic3.Capture_Period * (0XFFFF+1) + 
											 (ultrasonic3.Capture_CcrValue+1);
								ultrasonic3.distance=ultrasonic3.time%TIM_PscCLK*340/2/1000;
								if(ultrasonic3.distance>1500)
									ultrasonic3.distance=1500;
								// 打印高电平脉宽时间
								//printf ( "\r\n测得高电平脉宽时间：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );
								//Lcd_printf4(ultrasonic3.distance,0);
								printf ( "\r\n测量距离3：%d mm\r\n",ultrasonic3.distance );
								
								ultrasonic3.Capture_FinishFlag = 0;			
							}	
}
void CSB4(void)
{
						if(ultrasonic4.Capture_FinishFlag == 1)
							{
								// 计算高电平时间的计数器的值
								ultrasonic4.time = ultrasonic4.Capture_Period * (0XFFFF+1) + 
											 (ultrasonic4.Capture_CcrValue+1);
								ultrasonic4.distance=ultrasonic4.time%TIM_PscCLK*340/2/1000;
								// 打印高电平脉宽时间
								//printf ( "\r\n测得高电平脉宽时间：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );
								//Lcd_printf4(ultrasonic4.distance,30);
								printf ( "\r\n测量距离4：%d mm\r\n",ultrasonic4.distance );
								ultrasonic4.Capture_FinishFlag = 0;			
							}	
}
/**/
