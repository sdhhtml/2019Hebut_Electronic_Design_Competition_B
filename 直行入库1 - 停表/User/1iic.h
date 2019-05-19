#ifndef   _1IIC_H
#define   _1IIC_H
#include "stdint.h"
#include "stm32f10x.h"
#define SCL_H1         GPIOD->BSRR = GPIO_Pin_4
#define SCL_L1         GPIOD->BRR  = GPIO_Pin_4
#define SDA_H1         GPIOD->BSRR = GPIO_Pin_5
#define SDA_L1         GPIOD->BRR  = GPIO_Pin_5
#define SDA_read1      ((GPIOD->IDR & GPIO_Pin_5)!=0)?1:0
//IO方向设置
 
#define SDA_IN1()  {GPIOD->CRL&=0XFF0FFFFF;GPIOD->CRL|=(u32)8<<28;}
#define SDA_OUT1() {GPIOD->CRL&=0XFF0FFFFF;GPIOD->CRL|=(u32)3<<28;}
void IIC_GPIO_Init1(void);        //初始化IIC的IO口				 
void IIC_Start1(void);			 //发送IIC开始信号
void IIC_Stop1(void);	  	  	 //发送IIC停止信号
void IIC_Ack1(void);				 //IIC发送ACK信号
void IIC_NAck1(void);			 //IIC不发送ACK信号
uint8_t IIC_WaitAck1(void); 		 //IIC等待ACK信号

void IIC_SendByte1(uint8_t data);  //IIC发送一个字节
uint8_t IIC_ReadByte1(uint8_t ack);//IIC读取一个字节

uint8_t IIC_ReadByteFromSlave1(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf);
uint8_t IIC_ReadMultByteFromSlave1(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t IIC_WriteByteToSlave1(uint8_t I2C_Addr,uint8_t reg,uint8_t buf);
uint8_t IIC_WriteMultByteToSlave1(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);


#endif

