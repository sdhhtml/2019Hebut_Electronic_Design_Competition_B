#include "motor.h"
#include "bsp_SysTick.h"
#include "led.h"
#include "imu.h"
#include "oled.h"
extern unsigned int MotorSpeed;
extern unsigned char motor_mode;
extern double uart5_angle;
extern FLY_TYPE FLY;
extern FLOAT_ANGLE Att_Angle;  
extern unsigned int CaptureNumberA1;//右
extern unsigned int CaptureNumberA2;//上
extern unsigned int CaptureNumberB1;
extern unsigned int CaptureNumberB2;
extern int quanju_x;
extern int quanju_y;
extern unsigned char qd_code;

extern float yaww;
extern unsigned short int JG_distance;
void chumen(void)
{
	OLED_ShowString(byte(11),line1,"s0",6);
//	while(0==status.start)
//	{
//		all_oled();
//	}
	OLED_ShowString(byte(11),line1,"s1",6);
	while(hongwaiflags.x<1)
	{
		MotorSpeed=200;
		motor_mode=3;
		all_oled();
	}
	motor_mode=0;
	
	Delay_ms(200);	
	while(PGin(3)==0&&PGin(2)==0)
	{
		MotorSpeed=230;
		motor_mode=2;
		all_oled();
	}
	motor_mode=0;
	Delay_ms(200);	
//	quanju_x+=CaptureNumberA2;
//	quanju_y+=CaptureNumberA1;
	CaptureNumberA1=0;
	CaptureNumberA2=0;
}
void order(void)//顺序
{
	while(hongwaiflags.x<3)//4
	{
		MotorSpeed=300;
		motor_mode=3;
		all_oled();
	}	
	while(hongwaiflags.x<4)//6
	{
		MotorSpeed=200;
		motor_mode=3;
		all_oled();
	}		
	motor_mode=0;
	while(PFin(0)!=1)
	{
		MotorSpeed=200;
		motor_mode=4;
		all_oled();
	}	
	motor_mode=0;
	Delay_ms(200);	
}
void quzhaoerweima(void)
{
	while(hongwaiflags.x<5)//6
	{
		MotorSpeed=300;
		motor_mode=3;
		all_oled();
	}	
	while(hongwaiflags.x<6)//7
	{
		MotorSpeed=200;
		motor_mode=3;
		all_oled();
	}		
	motor_mode=0;
	while(PFin(0)!=1)
	{
		MotorSpeed=200;
		motor_mode=4;
		all_oled();
	}	
	motor_mode=0;
	Delay_ms(200);
}
void all_oled(void)
{
						if(yaww<0)
		{
			OLED_ShowString(byte(8),line8,"YAW:-",6);
			yaww = -1*yaww;
		}else
		{
			OLED_ShowString(byte(8),line8,"YAW: ",6);
		}
			OLED_ShowNum(byte(12),line8,yaww*10,4,6); //航向角
		
				if(PFin(0)==0)OLED_ShowNum(byte(0),line2,0,1,6); //航向角	
					else OLED_ShowNum(byte(0),line2,1,1,6); //航向角	
				if(PFin(3)==0)OLED_ShowNum(byte(2),line2,0,1,6); //航向角	
					else OLED_ShowNum(byte(2),line2,1,1,6); //航向角					
				if(PFin(2)==0)OLED_ShowNum(byte(4),line2,0,1,6); //航向角	
					else OLED_ShowNum(byte(4),line2,1,1,6); //航向角		
	
				if(PGin(2)==0)OLED_ShowNum(byte(0),line3,0,1,6); //航向角	
					else OLED_ShowNum(byte(0),line3,1,1,6); //航向角		
				if(PGin(3)==0)OLED_ShowNum(byte(2),line3,0,1,6); //航向角	
					else OLED_ShowNum(byte(2),line3,1,1,6); //航向角	
				OLED_ShowNum(byte(0),line4,hongwaiflags.x,1,6); //航向角	
				OLED_ShowNum(byte(6),line4,hongwaiflags.y,1,6); //航向角		
				OLED_ShowNum(byte(0),line8,motor_mode,4,6); //航向角
				OLED_ShowNum(byte(0),line6,CaptureNumberA1,6,6); //Y
				OLED_ShowNum(byte(6),line6,CaptureNumberA2,6,6); //X								
				OLED_ShowNum(byte(0),line7,CaptureNumberB1,6,6); //X
				OLED_ShowNum(byte(6),line7,CaptureNumberB2,6,6); //Y		
		if(qd_code==0)
			OLED_ShowNum(byte(8),line3,000,4,6); //二维码
		else if(qd_code==1)
			OLED_ShowNum(byte(8),line3,123,4,6); //二维码
		else if(qd_code==2)
			OLED_ShowNum(byte(8),line3,132,4,6); //二维码		
		else if(qd_code==3)
			OLED_ShowNum(byte(8),line3,213,4,6); //二维码	
		else if(qd_code==4)
			OLED_ShowNum(byte(8),line3,231,4,6); //二维码	
		else if(qd_code==5)
			OLED_ShowNum(byte(8),line3,312,4,6); //二维码	
		else if(qd_code==6)
			OLED_ShowNum(byte(8),line3,321,4,6); //二维码			
		
//																	OLED_ShowNum(byte(8),line2,hongwaiflags.x_0,1,6); //航向角					
//													OLED_ShowNum(byte(10),line2,hongwaiflags.x_1,1,6); //航向角	
//													OLED_ShowNum(byte(12),line2,hongwaiflags.x_2,1,6); //航向角	
//													OLED_ShowNum(byte(14),line2,hongwaiflags.x_3,1,6); //航向角	
}
//
void red_123(void)//到达左取点123
{
	while(hongwaiflags.x>4)//4
	{
		MotorSpeed=250;
		motor_mode=4;
		//all_oled();
	}	
	motor_mode=0;
	Delay_ms(200);
	while(PGin(6)!=1)
	{
		MotorSpeed=200;
		motor_mode=3;
		all_oled();
	}	
	motor_mode=0;
	while(PGin(2)!=0||PGin(3)!=1)
	{
		motor_mode=2;
	}
	motor_mode=0;
}
void downred_123(void)//到达红色放置点123
{
	while(hongwaiflags.y<1)//2
	{
		MotorSpeed=y_speed;
		motor_mode=2;
		//all_oled();
	}
	while(hongwaiflags.y<2)//2
	{
		MotorSpeed=y_speed-20;
		motor_mode=2;
		//all_oled();
	}	
	//Delay_ms(50);
	motor_mode=0;
	Delay_ms(500);		
}

void returnred_123(void)//回到左取点123
{
	while(hongwaiflags.y>1)
	{
		MotorSpeed=y_speed;
		motor_mode=1;	
		//all_oled();
	}
	while(hongwaiflags.y>0)
	{
		MotorSpeed=200;
		motor_mode=1;	
		//all_oled();
	}		
//	while(PGin(2)==0&&PGin(3)==1)
//	{
//		motor_mode=2;
//	}
	motor_mode=0;
	Delay_ms(1000);	
	if((PGin(6)==0&&PGin(7)==0&&PGin(8)==0))
	{
		hongwaiflags.x+=1;
	}

}
void green_123(void)//到达中间取点123
{
	while(hongwaiflags.x>3)//3
	{
		MotorSpeed=250;
		motor_mode=4;
		//all_oled();
	}	
	while(PGin(6)!=1)
	{
		MotorSpeed=200;
		motor_mode=3;
		all_oled();
	}	
	motor_mode=0;	
	Delay_ms(200);
	while(PGin(2)!=0||PGin(3)!=1)
	{
		motor_mode=2;
	}
	motor_mode=0;
}
void downgreen_123(void)//到达绿色放置点123
{
	while(hongwaiflags.y<1)//2
	{
		MotorSpeed=y_speed;
		motor_mode=2;
		//all_oled();
	}
	while(hongwaiflags.y<2)//2
	{
		MotorSpeed=y_speed-20;
		motor_mode=2;
		//all_oled();
	}	
//	Delay_ms(50);
	motor_mode=0;
	Delay_ms(500);	
}
void returngreen_123(void)//回到中间取点
{
	while(hongwaiflags.y>1)
	{
		MotorSpeed=y_speed;
		motor_mode=1;	
		//all_oled();
	}
	while(hongwaiflags.y>0)
	{
		MotorSpeed=220;
		motor_mode=1;	
		//all_oled();
	}		
//	while(PGin(3)==0||PGin(2)==0)
//	{
//		motor_mode=2;
//	}
	motor_mode=0;
	Delay_ms(1000);	
	if((PGin(6)==0&&PGin(7)==0&&PGin(8)==0))
	{
		hongwaiflags.x+=1;
	}	
}
void blue_123(void)//到达中间取点123
{
	while(PGin(2)!=0||PGin(3)!=1)
	{
		motor_mode=2;
	}
	motor_mode=0;
}
void downblue_123(void)//到达蓝色放置点123
{
	while(hongwaiflags.x>2)//2
	{
		MotorSpeed=250;
		motor_mode=4;
		//all_oled();
	}	
	while(PGin(7)!=1)
	{
		MotorSpeed=200;
		motor_mode=3;
		//all_oled();
	}	
	motor_mode=0;
	Delay_ms(200);		
	while(hongwaiflags.y<1)//2
	{
		MotorSpeed=y_speed;
		motor_mode=2;
		//all_oled();
	}
	while(hongwaiflags.y<2)//2
	{
		MotorSpeed=y_speed-10;
		motor_mode=2;
		//all_oled();
	}	
	//Delay_ms(50);
	motor_mode=0;
	Delay_ms(500);	
}
void returnblue_123(void)//回到蓝色取点
{
	while(hongwaiflags.y>1)
	{
		MotorSpeed=y_speed ;
		motor_mode=1;	
		all_oled();
	}
	while(hongwaiflags.y>0)
	{
		MotorSpeed=220;
		motor_mode=1;	
		//all_oled();
	}		
//	while(PGin(2)!=0&&PGin(3)!=1)
//	{
//		motor_mode=2;
//	}
//	motor_mode=0;
//	Delay_ms(1000);	
	if((PGin(6)==0&&PGin(7)==0&&PGin(8)==0))
	{
		hongwaiflags.x+=1;
	}
}

void fromtake_tohome(void)
{
	while(hongwaiflags.x>0)
	{
		MotorSpeed=250;
		motor_mode=4;	
		//all_oled();
	}		
	motor_mode=0;
	MotorSpeed=220;
	motor_mode=4;
	Delay_ms(300);
	motor_mode=1;
	Delay_ms(300);	
	motor_mode=0;
}

void code_mid(void)//从二维码到中间
{
	while(hongwaiflags.x>3)//3
	{
		MotorSpeed=300;
		motor_mode=4;
		//all_oled();
	}	
	while(PFin(0)!=1)
	{
		MotorSpeed=200;
		motor_mode=3;
		all_oled();
	}	
	motor_mode=0;	
	Delay_ms(200);
	//while((PGin(2)!=0||PGin(3)!=1)&&(PGin(2)!=1&&PGin(3)!=1))
	if(PGin(2)==0&&PGin(3)==0)
	{
			while(PGin(2)!=0||PGin(3)!=1)
			{
				motor_mode=2;
			}
			motor_mode=0;	
	}
	
	else if(PGin(2)==1&&PGin(3)==1)
	{
			while(PGin(2)!=0||PGin(3)!=1)
			{
				motor_mode=1;
			}
			motor_mode=0;	
	}	
}
void mid_place(void)//从中间到放置zhongjian
{
	while(hongwaiflags.y<1)//2
	{
		MotorSpeed=y_speed;
		motor_mode=2;
		//all_oled();
	}
	while(hongwaiflags.y<2)//2
	{
		MotorSpeed=y_speed-20;
		motor_mode=2;
		//all_oled();
	}	
	Delay_ms(90);
	motor_mode=0;
	Delay_ms(500);
	if((PFin(3)==1||PFin(2)==1)&&PFin(0)!=1)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=3;
					//all_oled();
				}	
			}
	else if(PFin(3)==0&&PFin(2)==0&&PFin(0)!=1)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=4;
					//all_oled();
				}	
			}		
	motor_mode=0;	
	Delay_ms(200);				
}

void mid_place_bian(void)//从中间到放置bian
{
	while(hongwaiflags.y<1)//2
	{
		MotorSpeed=y_speed;
		motor_mode=2;
		//all_oled();
	}
	while(hongwaiflags.y<2)//2
	{
		MotorSpeed=y_speed-20;
		motor_mode=2;
		//all_oled();
	}	
	Delay_ms(200);
	motor_mode=0;
	Delay_ms(500);
//	if((PFin(3)==1||PFin(2)==1)&&PFin(0)!=1)
//			{
//				while(PFin(0)!=1)
//				{
//					MotorSpeed=220;
//					motor_mode=3;
//					//all_oled();
//				}	
//			}
//	else if(PFin(3)==0&&PFin(2)==0&&PFin(0)!=1)
//			{
//				while(PFin(0)!=1)
//				{
//					MotorSpeed=220;
//					motor_mode=4;
//					//all_oled();
//				}	
//			}		
//	motor_mode=0;	
	Delay_ms(200);				
}
void place_mid(void)//从放置到中间
{
	while(hongwaiflags.y>1)
	{
		MotorSpeed=y_speed;
		motor_mode=1;	
		//all_oled();
	}
	while(hongwaiflags.y>0)
	{
		MotorSpeed=y_speed-20;
		motor_mode=1;	
		//all_oled();
	}		
	motor_mode=0;
	Delay_ms(200);	
	while(PGin(2)!=0||PGin(3)!=1)///////////////////////////////////////////
	{
		motor_mode=2;
	}
	motor_mode=0;
	Delay_ms(200);		
	/////////////////////////////////
	if((PFin(3)==1||PFin(2)==1)&&PFin(0)!=1)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=3;
					//all_oled();
				}	
			}
	else if(PFin(3)==0&&PFin(2)==0&&PFin(0)!=1)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=4;
					//all_oled();
				}	
			}		
	motor_mode=0;	
	Delay_ms(200);	
}
void place_start(void)//从放置回初始点。
{
	if((PFin(0)==0&&PFin(3)==0&&PFin(2)==0))
	{
		hongwaiflags.x+=1;
	}		
	while(hongwaiflags.x>0)
	{
		MotorSpeed=250;
		motor_mode=4;	
		//all_oled();
	}		
	motor_mode=0;
	MotorSpeed=220;
	motor_mode=4;
	Delay_ms(300);
	motor_mode=1;
	Delay_ms(300);	
	motor_mode=0;	

}
void new_chumen(void)//激光出门测试1
{
	while(PGin(3)==0&&PGin(2)==0)
	{
		MotorSpeed=200;
		motor_mode=2;
	}
	motor_mode=0;
	Delay_ms(200);
	
}
void new_qrcode(void)//红外对管去找二维码
{
	while(hongwaiflags.x<5)//6
	{
		MotorSpeed=400;
		motor_mode=3;
	}	
	while(hongwaiflags.x<6)//7
	{
		MotorSpeed=250;
		motor_mode=3;
	}		
	motor_mode=0;
	while(PFin(0)!=1)
	{
		MotorSpeed=3000;
		motor_mode=4;
	}	
	motor_mode=0;
	//all_oled();
}
void new_qrget(void)//从二维码到中间抓取区
{
	if(PFin(0)!=1)
	hongwaiflags.x-=1;
	while(hongwaiflags.x>3)//4
	{
		MotorSpeed=250;
		motor_mode=4;
	}	
	motor_mode=0;
	Delay_ms(90);
	while(PFin(0)!=1)
	{
		MotorSpeed=200;
		motor_mode=3;
	}	
	motor_mode=0;	
	Delay_ms(200);
	if((PFin(3)==1||PFin(2)==1)&&PFin(0)!=1)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=3;
					//all_oled();
				}	
			}
	else if(PFin(3)==0&&PFin(2)==0&&PFin(0)==0)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=4;
					//all_oled();
				}	
			}		
	motor_mode=0;	
	Delay_ms(200);	
			motor_mode=1;	
			Delay_ms(600);
			motor_mode=0;	
			hongwaiflags.y=0;
	if(PGin(2)==0&&PGin(3)==0)
	{
			while(PGin(2)==0&&PGin(3)==0)
			{
				MotorSpeed=210;
				motor_mode=2;
			}
			motor_mode=0;	
	}
	motor_mode=0;
	Delay_ms(90);
	hongwaiflags.y=0;
}
void new_placemid(void)//从抓取到中间放置点
{
	while(hongwaiflags.y<1)//2
	{
		MotorSpeed=y_speed;
		motor_mode=2;
	}
	while(hongwaiflags.y<2)//2
	{
		MotorSpeed=y_speed-50;
		motor_mode=2;
	}	
	motor_mode=0;
	if((PFin(3)==1||PFin(2)==1)&&PFin(0)!=1)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=3;
				}	
			}
	else if(PFin(3)==0&&PFin(2)==0&&PFin(0)!=1)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=4;
				}	
			}		
	motor_mode=0;	
	Delay_ms(200);			
}
void new_placeedge(void)//从抓取到边缘放置点
{
	while(hongwaiflags.y<1)//2
	{
		MotorSpeed=y_speed+20;
		motor_mode=2;
	}
	while(hongwaiflags.y<2)//2
	{
		MotorSpeed=y_speed;
		motor_mode=2;
	}	
	motor_mode=0;
	if((PFin(3)==1||PFin(2)==1)&&PFin(0)!=1)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=3;
				}	
			}
	if(PFin(3)==0&&PFin(2)==0&&PFin(0)==0)
			{
				while(PFin(0)==0)
				{
					MotorSpeed=220;
					motor_mode=4;
				}	
			}		
	motor_mode=0;	
}
void new_getmid(void)//从放置点到中间抓取点
{
	while(hongwaiflags.y>1)
	{
		MotorSpeed=y_speed;
		motor_mode=1;	
	}
	while(hongwaiflags.y>0)
	{
		MotorSpeed=y_speed-20;
		motor_mode=1;	
	}		
	motor_mode=0;
	Delay_ms(200);
	while(PGin(2)==0&&PGin(3)==0)///////////////////////////////////////////
	{
		MotorSpeed=220;
		motor_mode=2;
	}
	motor_mode=0;
	Delay_ms(200);	

	if((PFin(3)==1||PFin(2)==1)&&PFin(0)!=1)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=3;
				}	
			}
	else if(PFin(3)==0&&PFin(2)==0&&PFin(0)==0)
			{
				while(PFin(0)!=1)
				{
					MotorSpeed=220;
					motor_mode=4;
				}	
			}		
	motor_mode=0;			
}
void gohome(void)
{
//	hongwaiflags.x-=1;
	if((PFin(0)==0&&PFin(3)==0&&PFin(2)==0))
	{
		hongwaiflags.x+=1;
	}		
	while(hongwaiflags.x>0)
	{
		MotorSpeed=500;
		motor_mode=4;	
	}		
	Delay_ms(150);

		motor_mode=1;
		MotorSpeed=250;

	Delay_ms(500);	
	motor_mode=0;		
}
/**/
