#include "key.h"
#include "led.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "mpu9250.h"
#include "mpu6050.h"
extern u16 tim1ch1;
extern u16 tim1ch4;
extern double angleoffset;
extern double uart5_angle;
//KEY	D3 D4 E7 D12 A12 F12 F13 F15
//D3 D4 D12 
//A12
//E7
//F12 F13 F15
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;  
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  /* 配置中断源：按键2，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel =EXTI3_IRQn;
  NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannel =EXTI4_IRQn;
  NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;
  NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannel =EXTI15_10_IRQn;
  NVIC_Init(&NVIC_InitStructure);	
}
void key_init(void)
{
	GPIO_InitTypeDef GPIO_initStructure;
	EXTI_InitTypeDef EXTI_initStructure;
	NVIC_Configuration();
	//时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_AFIO,ENABLE);	
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_initStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_initStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_initStructure);

	GPIO_initStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_12;	
	GPIO_Init(GPIOD,&GPIO_initStructure);
	
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOE,&GPIO_initStructure);
	
	GPIO_initStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_Init(GPIOF,&GPIO_initStructure);
	//给所用到的中断线路配置相应的触发模式和触发时机
	EXTI_initStructure.EXTI_Line = EXTI_Line3;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initStructure.EXTI_Trigger =EXTI_Trigger_Falling;
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_initStructure);
	
	EXTI_initStructure.EXTI_Line = EXTI_Line4;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_initStructure);
	
	EXTI_initStructure.EXTI_Line = EXTI_Line7;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_initStructure);
	
	EXTI_initStructure.EXTI_Line = EXTI_Line12;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_initStructure);

	EXTI_initStructure.EXTI_Line = EXTI_Line13;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_initStructure);
	
	EXTI_initStructure.EXTI_Line = EXTI_Line15;
	EXTI_initStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_initStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_initStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_initStructure);
	//开启GPIO管脚的中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource4);		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource7);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource15);
	
	//清除中断标志，防止一打开中断通道就进入中断
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line4);
	EXTI_ClearITPendingBit(EXTI_Line7);
	EXTI_ClearITPendingBit(EXTI_Line12);
	EXTI_ClearITPendingBit(EXTI_Line13);
	EXTI_ClearITPendingBit(EXTI_Line15);
	//开启NVIC中EXTI的中断通道
	NVIC_EnableIRQ(EXTI3_IRQn);	
	NVIC_EnableIRQ(EXTI4_IRQn);	
	NVIC_EnableIRQ(EXTI9_5_IRQn);	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}
//按键中断处理函数
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET){
		if((GPIOD->IDR & GPIO_Pin_3) == RESET){				//注意：这里不能用==SET来判断，因为SET = !RESET,也就是说SET的数字代表值是非零
			Delay_ms(10);										//延迟10ms
			if((GPIOD->IDR & GPIO_Pin_3) == RESET){			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
				while((GPIOD->IDR & GPIO_Pin_3) == RESET);		//等待按钮释放
				digitalToggle(GPIOE,GPIO_Pin_8);
				status.start=1;
			}
		}
	}
		EXTI_ClearITPendingBit(EXTI_Line7);	
}
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET){
		if((GPIOD->IDR & GPIO_Pin_4) == RESET){				//注意：这里不能用==SET来判断，因为SET = !RESET,也就是说SET的数字代表值是非零
			Delay_ms(10);										//延迟10ms
			if((GPIOD->IDR & GPIO_Pin_4) == RESET){			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
				while((GPIOD->IDR & GPIO_Pin_4) == RESET);		//等待按钮释放
				digitalToggle(GPIOE,GPIO_Pin_8);
				status.start=1;
			}
		}
	}
		EXTI_ClearITPendingBit(EXTI_Line7);	
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line7) == SET){
		if((GPIOE->IDR & GPIO_Pin_7) == RESET){				//注意：这里不能用==SET来判断，因为SET = !RESET,也就是说SET的数字代表值是非零
			Delay_ms(10);										//延迟10ms
			if((GPIOE->IDR & GPIO_Pin_7) == RESET){			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
				while((GPIOE->IDR & GPIO_Pin_7) == RESET);		//等待按钮释放
				digitalToggle(GPIOE,GPIO_Pin_8);
			}
		}
	}
		EXTI_ClearITPendingBit(EXTI_Line7);	
}



void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12) == SET){
		if((GPIOA->IDR & GPIO_Pin_12) == RESET){
			Delay_ms(10);										//延迟20ms
			if((GPIOA->IDR & GPIO_Pin_12) == RESET){			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
				while((GPIOA->IDR & GPIO_Pin_12) == RESET);
				digitalToggle(GPIOE,GPIO_Pin_8);
		}
		if((GPIOD->IDR & GPIO_Pin_12) == RESET){
			Delay_ms(10);										//延迟20ms
			if((GPIOD->IDR & GPIO_Pin_12) == RESET){			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
				while((GPIOD->IDR & GPIO_Pin_12) == RESET);
				digitalToggle(GPIOE,GPIO_Pin_9);
		}			
		if((GPIOF->IDR & GPIO_Pin_12) == RESET){
			Delay_ms(10);										//延迟20ms
			if((GPIOF->IDR & GPIO_Pin_12) == RESET){			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
				while((GPIOF->IDR & GPIO_Pin_12) == RESET);
				digitalToggle(GPIOE,GPIO_Pin_10);
		}
	}
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
		else if(EXTI_GetITStatus(EXTI_Line13) == SET){
		if((GPIOF->IDR & GPIO_Pin_13) == RESET){
			Delay_ms(10);										//延迟20ms
			if((GPIOF->IDR & GPIO_Pin_13) == RESET){			//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
				while((GPIOF->IDR & GPIO_Pin_13) == RESET);
				digitalToggle(GPIOE,GPIO_Pin_8);
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	else if(EXTI_GetITStatus(EXTI_Line15) == SET){
		if((GPIOF->IDR & GPIO_Pin_15)== RESET){
			Delay_ms(10);										//延迟20ms
			if((GPIOF->IDR & GPIO_Pin_15)== RESET){				//再检测一下按键的电平状态，若此时还是处于低电平状态，说明按键不是由于抖动触发的
				while((GPIOF->IDR & GPIO_Pin_15)== RESET);
				digitalToggle(GPIOE,GPIO_Pin_8);
				
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
}
	}
/**/
