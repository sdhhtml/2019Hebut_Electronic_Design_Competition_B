#ifndef   _HMC5983_H
#define   _HMC5983_H

#include "stm32f10x.h"
#define HMCSCL_H         GPIOD->BSRR = GPIO_Pin_4
#define HMCSCL_L         GPIOD->BRR  = GPIO_Pin_4
#define HMCSDA_H         GPIOD->BSRR = GPIO_Pin_5
#define HMCSDA_L         GPIOD->BRR  = GPIO_Pin_5
#define HMCSDA_read      ((GPIOD->IDR & GPIO_Pin_5)!=0)?1:0
//IO方向设置
 
#define HMCSDA_IN()  {GPIOD->CRL&=0XFF0FFFFF;GPIOD->CRL|=(u32)8<<20;}
#define HMCSDA_OUT() {GPIOD->CRL&=0XFF0FFFFF;GPIOD->CRL|=(u32)3<<20;}
void HMCIIC_GPIO_Init(void);        //初始化IIC的IO口				 

uint8_t HMCIIC_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf);
uint8_t HMCIIC_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data);
uint8_t HMCIIC_ReadMultByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t HMCIIC_WriteMultByteToSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
#endif
