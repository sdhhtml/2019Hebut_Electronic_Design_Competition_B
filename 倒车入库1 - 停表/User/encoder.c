#include "encoder.h"
#include "stm32f10x.h"
#include "key.h"
#include "led.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "mpu9250.h"
#include "mpu6050.h"
extern unsigned int CaptureNumberA1;//右
extern unsigned int CaptureNumberA2;//上
extern unsigned int CaptureNumberB1;
extern unsigned int CaptureNumberB2;
static void NVIC_Configuration1(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;  
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  /* 配置中断源：按键2，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;
  NVIC_Init(&NVIC_InitStructure);


}
void Encoder_init(void)
{
	GPIO_InitTypeDef GPIO_initStructure;
	EXTI_InitTypeDef EXTI_initStructure;
	NVIC_Configuration1();
	//时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);	
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_initStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_initStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_initStructure);
	//给所用到的中断线路配置相应的触发模式和触发时机
	EXTI_initStructure.EXTI_Line = EXTI_Line6;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initStructure.EXTI_Trigger =EXTI_Trigger_Rising;
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_initStructure);
	EXTI_initStructure.EXTI_Line = EXTI_Line7;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initStructure.EXTI_Trigger =EXTI_Trigger_Rising;
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_initStructure);
	//开启GPIO管脚的中断线路

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);	
	//清除中断标志，防止一打开中断通道就进入中断

	EXTI_ClearITPendingBit(EXTI_Line6);
	EXTI_ClearITPendingBit(EXTI_Line7);
	
	//开启NVIC中EXTI的中断通道

	NVIC_EnableIRQ(EXTI9_5_IRQn);

}
//void EXTI9_5_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line6) == SET){
//		if((GPIOA->IDR & GPIO_Pin_6) == SET){				//注意：这里不能用==SET来判断，因为SET = !RESET,也就是说SET的数字代表值是非零
////			if((GPIOA->IDR & GPIO_Pin_6) == SET){			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
//////				while((GPIOA->IDR & GPIO_Pin_6) == SET);		//等待按钮释放

////			}
//			CaptureNumberA1+=1;
//		}
//	}
//		EXTI_ClearITPendingBit(EXTI_Line7);	
//	
//	if(EXTI_GetITStatus(EXTI_Line7) == SET){
//		if((GPIOA->IDR & GPIO_Pin_7) == SET){				//注意：这里不能用==SET来判断，因为SET = !RESET,也就是说SET的数字代表值是非零
////			if((GPIOA->IDR & GPIO_Pin_6) == SET){			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
//////				while((GPIOA->IDR & GPIO_Pin_6) == SET);		//等待按钮释放

////			}
//			CaptureNumberA2+=1;
//		}
//	}
//		EXTI_ClearITPendingBit(EXTI_Line7);	
//}
//TIM3	CH1-A6 CH2-A7
