#include "steer.h"
#include "new_struct.h"
#include "openmv.h"
#include "motor.h"
unsigned char Openmv_Mode=0;

//extern unsigned char Openmv_Mode;//openmv模式
extern unsigned short int qd_code;//二维码
/*发车区→二维码
123
132
213
231
312
321
*/
extern unsigned char color_shunjian;//颜色瞬间
// 1 2 3
extern unsigned char order_1;//物块顺序
//1 2 3 4 5 6
extern unsigned char order_2;//色环顺序
extern unsigned short int color_mode;//色环顺序
/*发车区→二维码
123
132
213
231
312
321
*/
void Openmv_Init(void)
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/

	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOF, &GPIO_InitStructure);	


		/* 关闭所有led灯	*/
		PFout(4)=0;
		PFout(5)=0;
		PFout(6)=0;
		PFout(7)=0;
}
void OpenMv_00(void)//二维码
{
		PFout(4)=0;
		PFout(5)=0;	
		PFout(6)=0;
		PFout(7)=0;	
		Openmv_Mode=0;
}
void OpenMv_11(void)//wu
{
		PFout(4)=1;
		PFout(5)=1;	
			PFout(6)=1;
		PFout(7)=2;
		Openmv_Mode=3;
}
void OpenMv_01(void)//圆环单色
{
		PFout(4)=0;
		PFout(5)=1;	
		PFout(6)=0;
		PFout(7)=1;	
		Openmv_Mode=1;
}
void OpenMv_10(void)//三色
{
		PFout(4)=1;
		PFout(5)=0;	
			PFout(6)=1;
		PFout(7)=0;
		Openmv_Mode=2;
}
void getwk(int ch)
{
	if(ch==1)
	{
		Grasp_right();//(抓取右边发车方向）
		Unfetched_state();//准备抓取状态			
	}
	else if(ch==2)
	{
		Grasp_mid();//(抓取中间）
		Unfetched_state();//准备抓取状态		
	}
	else if(ch==3)
	{
		Grasp_left();//(抓取左边二维码方向）
		Unfetched_state();//准备抓取状态			
	}
}
void placewk(int ch)
{
	if(ch==3)
	{
		Place_left();//放置左边二维码方向
		Unfetched_state();//准备抓取状态		
	}
	else if(ch==2)
	{
		Place_mid();//放置中间
		Unfetched_state();//准备抓取状态	
	}
	else if(ch==1)
	{
		Place_right();//放置右边发车区方向
		Unfetched_state();//准备抓取状态		
	}	
}
void nine_mode(int ch)
{
	switch(ch){
		case 1:{//1→1
			getwk(1);
			new_placeedge();
			placewk(1);
			new_getmid();
			break;
		}
		case 2:{//1→2
			getwk(1);
			new_placemid();
			placewk(2);
			new_getmid();			
			break;
		}
		case 3:{//1→3
			getwk(1);
			new_placeedge();
			placewk(3);
			new_getmid();
			break;
		}
		case 4:{//2→1
			getwk(2);
			new_placeedge();
			placewk(1);
			new_getmid();
			break;
		}
		case 5:{//2→2
			getwk(2);
			new_placemid();
			placewk(2);
			new_getmid();
			break;
		}
		case 6:{//2→3
			getwk(2);
			new_placeedge();
			placewk(3);
			new_getmid();
			break;
		}
		case 7:{//3→1
			getwk(3);
			new_placeedge();
			placewk(1);
			new_getmid();
			break;
		}
		case 8:{//3→2
			getwk(3);
			new_placemid();
			placewk(2);
			new_getmid();			
			break;
		}
		case 9:{//3→3
			getwk(3);
			new_placeedge();
			placewk(3);
			new_getmid();
			break;
		}		
	}
}
void huizong(void)
{
	if(qd_code==123&&order_1==1&&color_mode==123)///123 1
	{
		nine_mode(1);
		nine_mode(5);
		nine_mode(9);
	}
	else if(qd_code==123&&order_1==1&&color_mode==132)
	{
		nine_mode(1);
		nine_mode(6);
		nine_mode(8);		
	}		
	else if(qd_code==123&&order_1==1&&color_mode==213)
	{
		nine_mode(2);
		nine_mode(4);
		nine_mode(9);		
	}		
	else if(qd_code==123&&order_1==1&&color_mode==231)
	{
		nine_mode(3);
		nine_mode(4);
		nine_mode(8);			 
	}	
	else if(qd_code==123&&order_1==1&&color_mode==312)
	{
		nine_mode(2);
		nine_mode(6);
		nine_mode(7);				
	}		
	else if(qd_code==123&&order_1==1&&color_mode==321)
	{
		nine_mode(3);
		nine_mode(5);
		nine_mode(7);				
	}		
}

