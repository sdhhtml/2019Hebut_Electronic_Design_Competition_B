#ifndef __OLED_H
#define __OLED_H 	   
//常用ASCII表
//偏移量32
//ASCII字符集
//偏移量32
//大小:12*6
#include "stm32f10x.h"
#include "stdlib.h"	    	
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		  0x00
#define XLevelH		  0x10
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xFF 
#define X_WIDTH 	  128
#define Y_WIDTH 	  64	   

/* OLED屏幕有7行 */
#define line1 0
#define line2 1
#define line3 2
#define line4 3
#define line5 4
#define line6 5
#define line7 6
#define line8 7
/* OLED屏幕有128的点,也就是16列（16*8） */
#define byte(data)  data*8
//-----------------OLED IIC端口定义----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_12)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define OLED_SCLK_Clr1() GPIO_ResetBits(GPIOB,GPIO_Pin_14)//SCL
#define OLED_SCLK_Set1() GPIO_SetBits(GPIOB,GPIO_Pin_14)

#define OLED_SDIN_Clr1() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//SDA
#define OLED_SDIN_Set1() GPIO_SetBits(GPIOB,GPIO_Pin_15) 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ClearBlue(void);
void OLED_ClearLine(u8 line);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,const char data[][16],u8 num);
void OLED_ShowCHinese7x7(u8 x,u8 y,const char data[][16],u8 num);
void OLED_ShowCHineseString(u8 x,u8 y,const char data[][16],u8 num);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture(void);
void OLEDIIC_Start(void);
void OLEDIIC_Stop(void);
void OLEDWrite_IIC_Command(unsigned char IIC_Command);
void OLEDWrite_IIC_Data(unsigned char IIC_Data);
void OLEDWrite_IIC_Byte(unsigned char IIC_Byte);

void OLEDIIC_Wait_Ack(void);
//
void OLED_WR_Byte1(unsigned dat,unsigned cmd);  
void OLED_Display_On1(void);
void OLED_Display_Off1(void);	   							   		    
void OLED_Init1(void);
void OLED_Clear1(void);
void OLED_ClearBlue1(void);
void OLED_ClearLine1(u8 line);
void OLED_DrawPoint1(u8 x,u8 y,u8 t);
void OLED_Fill1(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar1(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum1(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString1(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos1(unsigned char x, unsigned char y);

void fill_picture1(unsigned char fill_Data);
void Picture1(void);
void OLEDIIC_Start1(void);
void OLEDIIC_Stop1(void);
void OLEDWrite_IIC_Command1(unsigned char IIC_Command);
void OLEDWrite_IIC_Data1(unsigned char IIC_Data);
void OLEDWrite_IIC_Byte1(unsigned char IIC_Byte);

void OLEDIIC_Wait_Ack1(void);
#endif

