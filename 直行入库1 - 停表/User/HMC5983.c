#include "stm32f10x.h"
#include "HMC5983.h"
#include "bsp_SysTick.h"
#include <string.h>
#include <math.h>
#include "bsp_usart.h"
#include "structconfig.h"
#include "iic.h"
static uint8_t    HMC5983_buff[6];                  //加速度 陀螺仪 温度 原始数据
INT16_XYZ HMC5983;	     //地磁
void HMC5983_Init(void)
{
	
	IIC_WriteByteToSlave(HMC5983_ADDRESS,0x00,0x14);
	IIC_WriteByteToSlave(HMC5983_ADDRESS,0x02,0x00);

	
}
void HMC_Read(void)
{
	double angle=0; 
	Delay_ms(10);	
	IIC_ReadMultByteFromSlave(HMC5983_ADDRESS, 0x43,6, HMC5983_buff);
	HMC5983.X=HMC5983_buff[0]<<8|HMC5983_buff[1];
	HMC5983.Z=HMC5983_buff[2]<<8|HMC5983_buff[3];
	HMC5983.Y=HMC5983_buff[4]<<8|HMC5983_buff[5];
       if(HMC5983.X>0x7fff)HMC5983.X-=0xffff;	  
       if(HMC5983.Y>0x7fff)HMC5983.Y-=0xffff;	  
       angle= atan2(HMC5983.Y,HMC5983.X) * (180 / 3.14159265) + 180;  	
	printf("\r\n%lf\r\n",angle);
	
}	
//
