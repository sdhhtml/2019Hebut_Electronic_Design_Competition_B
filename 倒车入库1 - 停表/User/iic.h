#ifndef   _IIC_H
#define   _IIC_H
#include "stdint.h"
#include "stm32f10x.h"
#define SCL_H         GPIOD->BSRR = GPIO_Pin_6
#define SCL_L         GPIOD->BRR  = GPIO_Pin_6
#define SDA_H         GPIOD->BSRR = GPIO_Pin_7
#define SDA_L         GPIOD->BRR  = GPIO_Pin_7
#define SDA_read      ((GPIOD->IDR & GPIO_Pin_7)!=0)?1:0
//IO方向设置
#define SDA_IN()  {GPIOD->CRL&=0X0FFFFFFF;GPIOD->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOD->CRL&=0X0FFFFFFF;GPIOD->CRL|=(u32)3<<28;} 
//#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
//#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}
//#define MPU_SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
//#define MPU_SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}
void IIC_GPIO_Init(void);        //初始化IIC的IO口				 
void IIC_Start(void);			 //发送IIC开始信号
void IIC_Stop(void);	  	  	 //发送IIC停止信号
void IIC_Ack(void);				 //IIC发送ACK信号
void IIC_NAck(void);			 //IIC不发送ACK信号
uint8_t IIC_WaitAck(void); 		 //IIC等待ACK信号

void IIC_SendByte(uint8_t data);  //IIC发送一个字节
uint8_t IIC_ReadByte(uint8_t ack);//IIC读取一个字节

uint8_t IIC_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf);
uint8_t IIC_ReadMultByteFromSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);
uint8_t IIC_WriteByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t buf);
uint8_t IIC_WriteMultByteToSlave(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);


#endif

