#include "uart4.h"
#include "TIM4.h"
#include "led.h"
#include "TIM1.h"
extern unsigned char motor_mode;
//串口接收数组
unsigned char UART_RxBuffer4[UART_RX_BUFFER_SIZE4];
//串口接收数组指针
unsigned char UART_RxPtr4;
//extern unsigned char P_DATA;
 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration4(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void UART_Config4(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	// 打开串口外设的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = 115200;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(UART4, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC_Configuration4();
	
	// 使能串口接收中断
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(UART4, ENABLE);	    
}
void UART4_IRQHandler(void)
{
  unsigned char ucTemp;
	if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(UART4);
		if(ucTemp==0x30){motor_mode=0;}//0
		if(ucTemp==0x31){motor_mode=1;}//1
		if(ucTemp==0x32){motor_mode=2;}//2
		if(ucTemp==0x33){motor_mode=3;}//3
	  if(ucTemp==0x34){motor_mode=4;}//4
		if(ucTemp==0x35){status.TIM1CH1_up = 1;
		//P_DATA+=1;if(P_DATA>=30)P_DATA=30;IncPIDInit();//PID初始化
		}//5
		if(ucTemp==0x36){status.TIM1CH1_down=1;
		//P_DATA-=1;if(P_DATA<=0)P_DATA=0;IncPIDInit();//PID初始化
		}//6
		if(ucTemp==0x37){status.TIM1CH4_up=1;}//7
		if(ucTemp==0x38){status.TIM1CH4_down=1;}//8
		if(ucTemp==0x71){status.TIM8CH1_up=1;}//q
		if(ucTemp==0x77){status.TIM8CH1_down=1;}//w
		if(ucTemp==0x65){status.TIM8CH2_up=1;}//e
		if(ucTemp==0x72){status.TIM8CH2_down=1;}//r
//		if(ucTemp==0x39){status.TIM8CH1_up=1;}//9
//		LED2_TOGGLE;									
   // USART_SendData(UART4,ucTemp);    
	}	 
}

/**/
