#include "mpu9250.h"
#include "stdio.h"
#include "DataScope_DP.h"
#include "oled.h"
#include "stm32f10x.h"
#include "mpu9250.h"
#include "iic.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "led.h"
#include "stdio.h"
#include "structconfig.h"
#include "paramsave.h"
#include "math.h"
#include "imu.h"
uint8_t    MPU9250_buff[14];                  //加速度 陀螺仪 温度 原始数据
static uint8_t MAG_buff[6];
extern INT16_XYZ	 GYRO_OFFSET_RAW,ACC_OFFSET_RAW;		 //零漂数据
INT16_XYZ	 MPU9250_ACC_RAW,MPU9250_GYRO_RAW;	     //读取值原始数据
extern uint8_t    	 SENSER_OFFSET_FLAG;                     //传感器校准标志位
INT16_XYZ	 MAG_OFFSET_RAW;
INT16_XYZ MPU9250_MAG_RAW;	     //地磁
double angle=0;
int angle_off=0;
/*****************************************************************************
* 函  数：uint8_t MPU6050_WriteByte(uint8_t addr,uint8_t reg,uint8_t data)
* 功  能：写一个字节数据到 MPU6050 寄存器
* 参  数：reg： 寄存器地址
*         data: 要写入的数据
* 返回值：0成功 1失败
* 备  注：MPU6050代码移植只需把I2C驱动修改成自己的即可
*****************************************************************************/
uint8_t MPU9250_WriteByte(uint8_t reg,uint8_t data)
{
	if(IIC_WriteByteToSlave(MPU9250Addr,reg,data))
		return 1;
	else
		return 0;
}

/*****************************************************************************
* 函  数：uint8_t MPU6050_ReadByte(uint8_t reg,uint8_t *buf)
* 功  能：从指定MPU6050寄存器读取一个字节数据
* 参  数：reg： 寄存器地址
*         buf:  读取数据存放的地址
* 返回值：1失败 0成功
* 备  注：MPU6050代码移植只需把I2C驱动修改成自己的即可
*****************************************************************************/
uint8_t MPU9250_ReadByte(uint8_t reg,uint8_t *buf)
{
	if(IIC_ReadByteFromSlave(MPU9250Addr,reg,buf))
		return 1;
	else
		return 0;
}

/*****************************************************************************
* 函  数：uint8_t MPU6050_WriteMultBytes(uint8_t reg,uint8_t len,uint8_t *buf)
* 功  能：从指定寄存器写入指定长度数据
* 参  数：reg：寄存器地址
*         len：写入数据长度 
*         buf: 写入数据存放的地址
* 返回值：0成功 1失败
* 备  注：MPU6050代码移植只需把I2C驱动修改成自己的即可
*****************************************************************************/
uint8_t MPU9250_WriteMultBytes(uint8_t reg,uint8_t len,uint8_t *buf)
{
	if(IIC_WriteMultByteToSlave(MPU9250Addr,reg,len,buf))
		return 1;
	else
		return 0;
}

/*****************************************************************************
* 函  数：uint8_t MPU6050_ReadMultBytes(uint8_t reg,uint8_t len,uint8_t *buf)
* 功  能：从指定寄存器读取指定长度数据
* 参  数：reg：寄存器地址
*         len：读取数据长度 
*         buf: 读取数据存放的地址
* 返回值：0成功 0失败
* 备  注：MPU6050代码移植只需把I2C驱动修改成自己的即可
*****************************************************************************/
uint8_t MPU9250_ReadMultBytes(uint8_t reg,uint8_t len,uint8_t *buf)
{
	if(IIC_ReadMultByteFromSlave(MPU9250Addr,reg,len,buf))
		return 1;
	else
		return 0;
}
uint8_t MAG_ReadMultBytes(uint8_t reg,uint8_t len,uint8_t *buf)
{
	if(IIC_ReadMultByteFromSlave(MAG_ADDRESS,reg,len,buf))
		return 1;
	else
		return 0;
}
/*============================以上代码移植时需要修改=========================*/

/******************************************************************************
* 函  数：uint8_tMPU6050_getDeviceID(void)
* 功  能：读取  MPU6050 WHO_AM_I 标识将返回 0x68
* 参  数：无
* 返回值：返回读取数据
* 备  注：无
*******************************************************************************/
uint8_t MPU9250_getDeviceID(void)
{
	uint8_t buf;
	MPU9250_ReadByte(MPU9250_RA_WHO_AM_I, &buf);

	printf("%d\r\n",buf);
	return buf;
}

/******************************************************************************
* 函  数：uint8_tMPU6050_testConnection(void)
* 功  能：检测MPU6050 是否已经连接
* 参  数：无
* 返回值：1已连接 0未链接
* 备  注：无
*******************************************************************************/
uint8_t MPU9250_testConnection(void) 
{
	if(MPU9250_getDeviceID() == 104)  

		
		return 1;
	else 
		return 0;
}

/******************************************************************************
* 函  数：void MPU6050_Check()
* 功  能：检测IIC总线上的MPU6050是否存在
* 参  数：无
* 返回值：无
* 备  注：无
*******************************************************************************/
void MPU9250_Check(void) 
{ 
	while(!MPU9250_testConnection())
	{
		printf("\rMPU9250 no connect...\r\n");
//		RGB_LED_green(); //绿灯常亮
	}
}

/******************************************************************************
* 函  数：void MPU6050_AccRead(int16_t *accData)
* 功  能：读取加速度的原始数据
* 参  数：*accData 原始数据的指针
* 返回值：无
* 备  注：无
*******************************************************************************/
void MPU9250_AccRead(int16_t *accData)
{
    uint8_t buf[6];
   	MPU9250_ReadMultBytes(MPU9250_RA_ACCEL_XOUT_H,6,buf);
    accData[0] = (int16_t)((buf[0] << 8) | buf[1]);
    accData[1] = (int16_t)((buf[2] << 8) | buf[3]);
    accData[2] = (int16_t)((buf[4] << 8) | buf[5]);
}

/******************************************************************************
* 函  数：void MPU6050_GyroRead(int16_t *gyroData)
* 功  能：读取陀螺仪的原始数据
* 参  数：*gyroData 原始数据的指针
* 返回值：无
* 备  注：无
*******************************************************************************/
void MPU9250_GyroRead(int16_t *gyroData)
{
    uint8_t buf[6];
		MPU9250_ReadMultBytes(MPU9250_RA_GYRO_XOUT_H, 6, buf);
    gyroData[0] = (int16_t)((buf[0] << 8) | buf[1]) ;
    gyroData[1] = (int16_t)((buf[2] << 8) | buf[3]) ;
    gyroData[2] = (int16_t)((buf[4] << 8) | buf[5]) ;
}

/******************************************************************************
* 函  数：void MPU6050_TempRead(float *tempdata)
* 功  能：温度值读取
* 参  数：*tempdata 温度数据的指针
* 返回值：无
* 备  注：无
*******************************************************************************/
void MPU9250_TempRead(float *tempdata)
{
	uint8_t buf[2];
	short data;
	MPU9250_ReadMultBytes(MPU9250_RA_TEMP_OUT_H, 2, buf);
	data = (int16_t)((buf[0] << 8) | buf[1]) ;
	*tempdata = 36.53f + ((float)data/340.0f);
}
/******************************************************************************
* 函  数：void MPU6050_MagRead(int16_t *gyroData)
* 功  能：读取陀螺仪的原始数据
* 参  数：*gyroData 原始数据的指针
* 返回值：无
* 备  注：无
*******************************************************************************/
void MPU9250_MagRead(int16_t *magData)
{
    uint8_t buf[6];
		IIC_WriteByteToSlave(MPU9250Addr,0x37,0x02);
		Delay_ms(10);
		IIC_WriteByteToSlave(MAG_ADDRESS,0x0A,0x01);
		Delay_ms(10);	
		MAG_ReadMultBytes(MAG_XOUT_L, 6, buf);
    magData[0] = (int16_t)((buf[1] << 8) | buf[0]) ;
    magData[1] = (int16_t)((buf[3] << 8) | buf[2]) ;
    magData[2] = (int16_t)((buf[5] << 8) | buf[4]) ;
}
/******************************************************************************
* 函  数：void MPU6050_Init(void)
* 功  能：初始化MPU6050进入工作状态
* 参  数：无
* 返回值：无
* 备  注：DLPF 最好设为采样频率的一半！！！
*******************************************************************************/
/*
void MPU9250_Init(void)
{

	MPU9250_Check(); //检查MPU6050是否连接

	MPU9250_WriteByte(MPU9250_RA_PWR_MGMT_1, 0x80); //复位MPU6050
	Delay_ms(100);
	MPU9250_WriteByte(MPU9250_RA_PWR_MGMT_1, 0x00); //唤醒MPU6050，并选择陀螺仪x轴PLL为时钟源
	MPU9250_WriteByte(0x19, 0x00);
	MPU9250_WriteByte(0X1A,0x04);
//	MPU9250_WriteByte(MPU9250_RA_INT_ENABLE, 0x00); //禁止中断
	MPU9250_WriteByte(MPU9250_RA_GYRO_CONFIG, 0x18); //陀螺仪满量程+-2000度/秒 (最低分辨率 = 2^15/2000 = 16.4LSB/度/秒 
	MPU9250_WriteByte(MPU9250_RA_ACCEL_CONFIG, 0x10); //加速度满量程+-4g   (最低分辨率 = 2^15/4g = 8196LSB/g )
	MPU9250_WriteByte(0x6C,0X00);
//	MPU9250_WriteByte(MPU9250_RA_CONFIG, MPU9250_DLPF_BW_20);//设置陀螺的输出为1kHZ,DLPF=20Hz 
//	MPU9250_WriteByte(MPU9250_RA_SMPLRT_DIV, 0x00);  //采样分频 (采样频率 = 陀螺仪输出频率 / (1+DIV)，采样频率1000hz）
//	MPU9250_WriteByte(MPU9250_RA_INT_PIN_CFG, 0x02); //MPU 可直接访问MPU6050辅助I2C
//	MPU9250_WriteByte(0x6a, 0x00); 
	
	GYRO_Offset();


}
*/
/******************************************************************************
* 函  数：void MPU6050_CalOff(void)
* 功  能：陀螺仪加速度校准
* 参  数：无
* 返回值：无
* 备  注：无
*******************************************************************************/
void MPU9250_CalOff(void)
{

	 SENSER_FLAG_SET(ACC_OFFSET);//加速度校准
	 SENSER_FLAG_SET(GYRO_OFFSET);//陀螺仪校准
}
void MAG_CalOff(void)
{

	 SENSER_FLAG_SET(MAG_OFFSET);//加速度校准
}
/******************************************************************************
* 函  数：void MPU6050_CalOff_Acc(void)
* 功  能：加速度计校准
* 参  数：无
* 返回值：无
* 备  注：无
*******************************************************************************/
void MPU9250_CalOff_Acc(void)
{
	 SENSER_FLAG_SET(ACC_OFFSET);//加速度校准
}

/******************************************************************************
* 函  数：void MPU6050_CalOff_Gyr(void)
* 功  能：陀螺仪校准
* 参  数：无
* 返回值：无
* 备  注：无
*******************************************************************************/
void MPU9250_CalOff_Gyr(void)
{
	 SENSER_FLAG_SET(GYRO_OFFSET);//陀螺仪校准
}

/******************************************************************************
* 函  数：void MPU6050_Read(void)
* 功  能：读取陀螺仪加速度计的原始数据
* 参  数：无
* 返回值：无
* 备  注：无
*******************************************************************************/
void MPU9250_Read(void)
{
//	int iii;
	MPU9250_ReadMultBytes(MPU9250_RA_ACCEL_XOUT_H, 14, MPU9250_buff);// 查询法读取MPU6050的原始数据
//	for(iii=0;iii<14;iii++)
//	{
	//printf("%c\r\n",MPU6050_buff[iii]);
//	}
	//printf("sss\r\n");
}
void MAG_Read(void)
{
//	int iii;
	IIC_WriteByteToSlave(MPU9250Addr,0x37,0x02);
	Delay_ms(10);
	IIC_WriteByteToSlave(MAG_ADDRESS,0x0A,0x01);
	Delay_ms(10);		
	MAG_ReadMultBytes(MAG_XOUT_L, 6, MAG_buff);// 查询法读取MPU6050的原始数据
	
//	for(iii=0;iii<14;iii++)
//	{
	//printf("%c\r\n",MPU6050_buff[iii]);
//	}
	//printf("sss\r\n");
}
/******************************************************************************
* 函  数：uint8_t MPU6050_OffSet(INT16_XYZ value,INT16_XYZ *offset,uint16_t sensivity)
* 功  能：MPU6050零偏校准
* 参  数：value： 	 MPU6050原始数据
*         offset：	 校准后的零偏值
*         sensivity：加速度计的灵敏度
* 返回值：1校准完成 0校准未完成
* 备  注：无
*******************************************************************************/
uint8_t MPU9250_OffSet(INT16_XYZ value,INT16_XYZ *offset,uint16_t sensivity)
{
	static int32_t tempgx=0,tempgy=0,tempgz=0; 
	static uint16_t cnt_a=0;//使用static修饰的局部变量，表明次变量具有静态存储周期，也就是说该函数执行完后不释放内存
	if(cnt_a==0)
	{
		value.X=0;
		value.Y=0;
		value.Z=0;
		tempgx = 0;
		tempgy = 0;
		tempgz = 0;
		cnt_a = 1;
		sensivity = 0;
		offset->X = 0;
		offset->Y = 0;
		offset->Z = 0;
	}
	tempgx += value.X;
	tempgy += value.Y; 
	tempgz += value.Z-sensivity ;//加速度计校准 sensivity 等于 MPU6050初始化时设置的灵敏度值（8196LSB/g）;陀螺仪校准 sensivity = 0；
	if(cnt_a==200)               //200个数值求平均
	{
		offset->X=tempgx/cnt_a;
		offset->Y=tempgy/cnt_a;
		offset->Z=tempgz/cnt_a;
		cnt_a = 0;
		return 1;
	}
	cnt_a++;
	return 0;
}	
void MPU9250_MAG_OffSet(void)
{

   char count,i,j;
   int32_t 	value_buf[36];
   int32_t  sum1=0,sum2=0,sum3=0,temp=0;
   for  (count=0;count<N;count++)
   {
		 MAG_Read();
		 value_buf[count]=((((int16_t)MAG_buff[1]) << 8) | MAG_buff[0]) - MAG_OFFSET_RAW.X;
		 value_buf[count+12]=((((int16_t)MAG_buff[3]) << 8) | MAG_buff[2]) - MAG_OFFSET_RAW.Y;
		 value_buf[count+24]=((((int16_t)MAG_buff[5]) << 8) | MAG_buff[4]) - MAG_OFFSET_RAW.Z;
   }
    for (j=0;j<N-1;j++)
   {
      for (i=0;i<N-j;i++)
      {
         if ( value_buf[i]>value_buf[i+1] )
         {
            temp = value_buf[i];
            value_buf[i] = value_buf[i+1]; 
             value_buf[i+1] = temp;
         }
         if ( value_buf[i+12]>value_buf[i+13] )
         {
            temp = value_buf[i+12];
            value_buf[i+12] = value_buf[i+13]; 
             value_buf[i+13] = temp;
         }
         if ( value_buf[i+24]>value_buf[i+25] )
         {
            temp = value_buf[i+24];
            value_buf[i+24] = value_buf[i+25]; 
             value_buf[i+25] = temp;
         }				 
      }
   }

   for(count=1;count<N-1;count++)
	 {
			sum1 += value_buf[count];
			sum2 += value_buf[count+12];
			sum3 += value_buf[count+24];
	 }
   MAG_OFFSET_RAW.X=sum1/(N-2);
	 MAG_OFFSET_RAW.Y=sum2/(N-2);
	 MAG_OFFSET_RAW.Z=sum3/(N-2);
}	

/******************************************************************************
* 函  数：void MPU6050_DataProcess(void)
* 功  能：对MPU6050进行去零偏处理
* 参  数：无
* 返回值：无
* 备  注：无
*******************************************************************************/
void MPU9250_Offset(void)
{
	//加速度去零偏AD值 
	MPU9250_ACC_RAW.X =((((int16_t)MPU9250_buff[0]) << 8) | MPU9250_buff[1]) - ACC_OFFSET_RAW.X;
	MPU9250_ACC_RAW.Y =((((int16_t)MPU9250_buff[2]) << 8) | MPU9250_buff[3]) - ACC_OFFSET_RAW.Y;
	MPU9250_ACC_RAW.Z =((((int16_t)MPU9250_buff[4]) << 8) | MPU9250_buff[5]) - ACC_OFFSET_RAW.Z;
	//陀螺仪去零偏AD值 
	MPU9250_GYRO_RAW.X =((((int16_t)MPU9250_buff[8]) << 8) | MPU9250_buff[9]) - GYRO_OFFSET_RAW.X;
	MPU9250_GYRO_RAW.Y =((((int16_t)MPU9250_buff[10]) << 8) | MPU9250_buff[11]) - GYRO_OFFSET_RAW.Y;
	MPU9250_GYRO_RAW.Z =((((int16_t)MPU9250_buff[12]) << 8) | MPU9250_buff[13]) - GYRO_OFFSET_RAW.Z;
	
	if(GET_FLAG(GYRO_OFFSET)) //陀螺仪进行零偏校准
	{
		if(MPU9250_OffSet(MPU9250_GYRO_RAW,&GYRO_OFFSET_RAW,0))
		{
			
			 SENSER_FLAG_RESET(GYRO_OFFSET);
		//	 PID_WriteFlash(); //保存陀螺仪的零偏数据
			 //GYRO_Offset_LED();
		     SENSER_FLAG_SET(ACC_OFFSET);//校准加速度
			
//			 printf("GYRO_OFFSET_RAW Value :X=%d  Y=%d  Z=%d\n",GYRO_OFFSET_RAW.X,GYRO_OFFSET_RAW.Y,GYRO_OFFSET_RAW.Z);
//			 printf("\n");
		}
	}
	if(GET_FLAG(ACC_OFFSET)) //加速度计进行零偏校准 
	{
		if(MPU9250_OffSet(MPU9250_ACC_RAW,&ACC_OFFSET_RAW,8196))
		{
			 SENSER_FLAG_RESET(ACC_OFFSET);
			// PID_WriteFlash(); //保存加速度计的零偏数据
			// ACC_Offset_LED();
//			 printf("ACC_OFFSET_RAW Value X=%d  Y=%d  Z=%d\n",ACC_OFFSET_RAW.X,ACC_OFFSET_RAW.Y,ACC_OFFSET_RAW.Z); 
//			 printf("\n");
		}
	}
}

void MAG_Offset(void)
{
	unsigned char ii;          //计数变量
	unsigned char Send_Count; //串口需要发送的数据个数	
	MAG_Read();
	MPU9250_MAG_RAW.X =((((int16_t)MAG_buff[1]) << 8) | MAG_buff[0]) - MAG_OFFSET_RAW.X;
	MPU9250_MAG_RAW.Y =((((int16_t)MAG_buff[3]) << 8) | MAG_buff[2]) - MAG_OFFSET_RAW.Y;
	MPU9250_MAG_RAW.Z =((((int16_t)MAG_buff[5]) << 8) | MAG_buff[4]) - MAG_OFFSET_RAW.Z;
/*
	DataScope_Get_Channel_Data( MPU9250_MAG_RAW.X , 1 );  //将数据 1.0  写入通道 1
	DataScope_Get_Channel_Data( MPU9250_MAG_RAW.Y , 2 );  //将数据 2.0  写入通道 2
	DataScope_Get_Channel_Data( MPU9250_MAG_RAW.Z , 3 );  //将数据 3.0  写入通道 3	
	Send_Count = DataScope_Data_Generate(3); //生成3个通道的 格式化帧数据，返回帧数据长度
	for( ii = 0 ; ii < Send_Count; ii++)  //循环发送,直到发送完毕   
	{
		while((USART1->SR&0X40)==0); 
			USART1->DR = DataScope_OutPut_Buffer[ii]; //从串口丢一个字节数据出去      
	 }	
*/
}
void mag_once(void)
{
	int16_t 	value_buf[2];
	double angle1=0;
	IIC_WriteByteToSlave(MPU9250Addr,0x37,0x02);
	Delay_ms(10);
	IIC_WriteByteToSlave(MAG_ADDRESS,0x0A,0x01);
	Delay_ms(10);	
	
	MAG_ReadMultBytes(MAG_XOUT_L, 4, MAG_buff);// 查询法读取MPU6050的原始数据	
	value_buf[0]=((((int16_t)MAG_buff[1]) << 8) | MAG_buff[0]) - MAG_OFFSET_RAW.X;
	value_buf[1]=((((int16_t)MAG_buff[3]) << 8) | MAG_buff[2]) - MAG_OFFSET_RAW.Y;
	angle1= atan2(value_buf[1],value_buf[0]) * (180 / 3.14159265) + 180;
	if(angle1<0)
	{
		OLED_ShowString(byte(0),line6,"ANGLE1:-",6);
		angle1 = -1*angle1;
	}else
	{
		OLED_ShowString(byte(0),line6,"ANGLE1:",6);
		angle1=angle1;
	}	 

		OLED_ShowNum(byte(6),line6,angle1,3,6); //航向角	
}

void medianfilter(void)
{
	
	unsigned char ii;          //计数变量
	unsigned char Send_Count; //串口需要发送的数据个数	
	
   char count,i,j;
   int16_t 	value_buf[36];
   int16_t  sum1=0,sum2=0,sum3=0,temp=0;
	 int16_t MAGY;
   for  (count=0;count<N;count++)
   {
		 MAG_Read();
		 value_buf[count]=((((int16_t)MAG_buff[1]) << 8) | MAG_buff[0]) - MAG_OFFSET_RAW.X;
		 value_buf[count+12]=((((int16_t)MAG_buff[3]) << 8) | MAG_buff[2]) - MAG_OFFSET_RAW.Y;
		 value_buf[count+24]=((((int16_t)MAG_buff[5]) << 8) | MAG_buff[4]) - MAG_OFFSET_RAW.Z;
   }
    for (j=0;j<N-1;j++)
   {
      for (i=0;i<N-j;i++)
      {
         if ( value_buf[i]>value_buf[i+1] )
         {
            temp = value_buf[i];
            value_buf[i] = value_buf[i+1]; 
             value_buf[i+1] = temp;
         }
         if ( value_buf[i+12]>value_buf[i+13] )
         {
            temp = value_buf[i+12];
            value_buf[i+12] = value_buf[i+13]; 
             value_buf[i+13] = temp;
         }
         if ( value_buf[i+24]>value_buf[i+25] )
         {
            temp = value_buf[i+24];
            value_buf[i+24] = value_buf[i+25]; 
             value_buf[i+25] = temp;
         }				 
      }
   }

   for(count=1;count<N-1;count++)
	 {
			sum1 += value_buf[count];
			sum2 += value_buf[count+12];
			sum3 += value_buf[count+24];
	 }
   MPU9250_MAG_RAW.X=sum1/(N-2);
	 MPU9250_MAG_RAW.Y=sum2/(N-2);
	 MPU9250_MAG_RAW.Z=sum3/(N-2);
 #if 1
   angle= atan2(MPU9250_MAG_RAW.Y,MPU9250_MAG_RAW.X) * (180 / 3.14159265) + 180; 	 
//	 printf("\r\n%lf\r\n",angle);
#endif	 
		if(angle<0)
		{
			OLED_ShowString(byte(0),line8,"MAGY:-",6);
			MAGY = -1*angle;
		}else
		{
			OLED_ShowString(byte(0),line8,"MAGY:",6);
			MAGY=angle;
		}	 
		
		OLED_ShowNum(byte(6),line8,MAGY,3,6); //航向角
	 status.MAG_flag=1;
#if 0
//	 printf("x=%d\r\ny=%d\r\nz=%d\r\n",MPU9250_MAG_RAW.X,MPU9250_MAG_RAW.Y,MPU9250_MAG_RAW.Z);
	 
	DataScope_Get_Channel_Data( MPU9250_MAG_RAW.X , 1 );  //将数据 1.0  写入通道 1
	DataScope_Get_Channel_Data( MPU9250_MAG_RAW.Y , 2 );  //将数据 2.0  写入通道 2
	DataScope_Get_Channel_Data( MPU9250_MAG_RAW.Z , 3 );  //将数据 3.0  写入通道 3	
	Send_Count = DataScope_Data_Generate(3); //生成3个通道的 格式化帧数据，返回帧数据长度
	for( ii = 0 ; ii < Send_Count; ii++)  //循环发送,直到发送完毕   
	{
		while((USART1->SR&0X40)==0); 
			USART1->DR = DataScope_OutPut_Buffer[ii]; //从串口丢一个字节数据出去      
	 }	
#endif	
	 
}
void angle_offset(void)
{
	angle_off=angle;
}
void mpu9250quling(void)
{
}
/**/
