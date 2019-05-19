#include "TIM1.h"
#include "led.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "oled.h"
#include "steer.h"
void Observation_center(void)//观察中间
{
	Arbitrary_steering_gear(380,1);
	Delay_ms(500);
	Arbitrary_steering_gear(700,3);
	Delay_ms(500);
	Arbitrary_steering_gear(515,2);
	Delay_ms(500);
}
void Observe_the_right_side(void)//观察右边
{
	Arbitrary_steering_gear(515,2);
	Arbitrary_steering_gear(435,1);
	Delay_ms(500);
	//Arbitrary_steering_gear(495,3);
}
void Color_reset(void)//观察复位
{
	Arbitrary_steering_gear(670,2);
	Arbitrary_steering_gear(610,3);
	Arbitrary_steering_gear(560,1);
}
void Reading_Two_Dimensional_Code(void)//读取二维码
{
	Arbitrary_steering_gear(770,1);
	Arbitrary_steering_gear(360,3);
	Arbitrary_steering_gear(1050,2);
}
void Read_reset(void)//读取复位 
{
	Arbitrary_steering_gear(1140,2);
	Arbitrary_steering_gear(350,3);
	Arbitrary_steering_gear(1070,1);
}
void Grab_left(void)//抓取左边
{
	Arbitrary_steering_gear(850,1);
	Arbitrary_steering_gear(510,3);
	Arbitrary_steering_gear(750,2);
	Delay_ms(100);
	Arbitrary_steering_gear(360,4);
	Delay_ms(500);
	Arbitrary_steering_gear(520,2);
	Arbitrary_steering_gear(660,1);
}
void Drop_left(void)//放下左边
{
	//Arbitrary_steering_gear(660,1); 
	Arbitrary_steering_gear(675,3);
	Arbitrary_steering_gear(345,2);
	Arbitrary_steering_gear(520,4);
}
void Left_reduction(void)//左边复位
{
	Arbitrary_steering_gear(450,2);
	Arbitrary_steering_gear(410,3);
	Arbitrary_steering_gear(480,1);
	Arbitrary_steering_gear(700,2);
}
void Grasp_Center(void)//抓取中间
{
	Arbitrary_steering_gear(305,1);
	Arbitrary_steering_gear(565,3);
	Delay_ms(500);
	Arbitrary_steering_gear(410,2);
	Delay_ms(100);
	Arbitrary_steering_gear(360,4);
	Delay_ms(500);

}
void Drop_middle(void)//放下中间
{
	Arbitrary_steering_gear(660,1);
	Arbitrary_steering_gear(625,3);
	Arbitrary_steering_gear(365,2);
	Arbitrary_steering_gear(520,4);	
}
void Intermediate_reset(void)//中间复位
{
	Arbitrary_steering_gear(450,2);
	Arbitrary_steering_gear(410,3);
	Arbitrary_steering_gear(480,1);
	Arbitrary_steering_gear(700,2);
}
void Unfetched_state(void)//准备抓取状态
{
	Arbitrary_steering_gear(840,2);
}
