#ifndef   _HMC5983_H
#define   _HMC5983_H

#include "stm32f10x.h"

#define _FAIL 			1
#define _SUCCESS 	0


typedef struct HMC5883L_DATA_T
{
		int16_t vRawADC[3];							//传感器原始数据
		int16_t	vOFFSet[3];							//传感器校准偏移
		int16_t	vRealADC[3];						//传感器校准后的数据
		float temp_quantity[3];
		
}HMC5883L_DATA_T;


/** @defgroup iic地址及寄存器地址
  * @{
  */

#define HMC5983_ADDRESS			 0x3C

#define ConfigRegA           0x00
#define ConfigRegB           0x01
#define ModeRegister         0x02
#define MAG_DATA_REGISTER 	 0x03
#define StatusReg						 0x09	
#define IDRegA 							 0x0A
#define IDRegB							 0x0B
#define IDRegC							 0x0C
	
/**
  * @} endof iic地址及寄存器地址
  */
	

/** @defgroup 传感器控制配置参数
  * @{
  */
#define magGain              0x20
#define PositiveBiasConfig   0x11
#define NegativeBiasConfig   0x12
#define NormalOperation      0x10

#define ContinuousConversion 0x00
#define SingleConversion     0x01

// ConfigRegA valid sample averaging for 5883L
#define SampleAveraging_1    0x00
#define SampleAveraging_2    0x01
#define SampleAveraging_4    0x02
#define SampleAveraging_8    0x03

// ConfigRegA valid data output rates for 5883L
#define DataOutputRate_0_75HZ 0x00
#define DataOutputRate_1_5HZ  0x01
#define DataOutputRate_3HZ    0x02
#define DataOutputRate_7_5HZ  0x03
#define DataOutputRate_15HZ   0x04
#define DataOutputRate_30HZ   0x05
#define DataOutputRate_75HZ   0x06
void HMC5983_Init(void);	
void HMC_Read(void);

#endif
