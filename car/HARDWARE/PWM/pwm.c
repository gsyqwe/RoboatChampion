/**
  ******************************************************************************
  * @file    pwm.c 
  * @author  Tang Zhixuan
  * @version V1.0
  * @date    2017/5/13
  ******************************************************************************
  */

#include "pwm.h"
#include "gpio.h"

extern Rstruct RVals;//来自遥控器，不断更新
int move_speed;
int rotate_speed;//占空比
int move_state,rotate_state;
extern float left_duty_cycle_cal,right_duty_cycle_cal;//算出的左右轮的占空比
int primary_value_LRx,primary_value_RRy;

void TIM1_PWM_Init(u16 arr,u16 psc)//PWM 频率=72000/(arr+1)KHz,取arr=35999，psc=1，PWM频率1kHz
{		 		
  GPIO_InitTypeDef  GPIO_InitStructure;	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频,0
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=0; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式 PWM1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
  DUTY1=0;
	DUTY2=0;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
 	TIM_CtrlPWMOutputs(TIM1,ENABLE); //高级定时器要用
}   


void move()
{
	switch(rotate_state)
	{
		case RIGHT:
			DUTY1=rotate_speed/10;
			DUTY1=rotate_speed/10;
		// IN1=0,IN2=1;
		  IN3=1,IN4=0;
		  IN1=0;IN2=0;////
		  break;
		case LEFT:
			DUTY1=rotate_speed/10;
			DUTY2=rotate_speed/10;
		  IN1=1,IN2=0;
		  IN3=0;IN4=0;////
		//  IN3=0,IN4=1;
		  break;
		case STRAIGHT:
			switch(move_state)
			{
				case FORWARD:
			   DUTY1=DUTY2=move_speed;
				 IN1=1,IN2=0;
		     IN3=1,IN4=0;
				  break;
				case BACKWARD:
			   DUTY1=DUTY2=move_speed;
				 IN1=0,IN2=1;
		     IN3=0,IN4=1;
				  break;
				case STOP:
					IN1=IN2=IN3=IN4=0;
				  break;
			}
			break;
		}
}

//旋转部分没有进行pid调节
//void get_speed_state()
//{
//	target_move_speed=MAX_SPEED*abs(RVals.RRy-primary_value_RRy)/2048;
//	rotate_speed=MAX_SPEED*0.1;
//	if(RVals.RRy>primary_value_RRy+REMOTE_ERROR)
//		move_state=FORWARD;
//	else if(RVals.RRy<primary_value_RRy-REMOTE_ERROR)
//		move_state=BACKWARD;
//	else 
//		move_state=STOP;
//	if(RVals.LRx>primary_value_LRx+REMOTE_ERROR)
//		rotate_state=RIGHT;
//	else if(RVals.LRx<primary_value_LRx-REMOTE_ERROR)
//			rotate_state=LEFT;
//	else
//		rotate_state=STRAIGHT;
//}

//开环版
void get_speed_state()
{
	move_speed=DUTY_MAX*fabs(RVals.RRy-primary_value_RRy)/2048;
	if(move_speed>DUTY_MAX) move_speed=DUTY_MAX;
	rotate_speed=DUTY_MAX*0.1;
	if(RVals.RRy>primary_value_RRy+REMOTE_ERROR)
		move_state=FORWARD;
	else if(RVals.RRy<primary_value_RRy-REMOTE_ERROR)
		move_state=BACKWARD;
	else 
		move_state=STOP;
	if(RVals.LRx>primary_value_LRx+REMOTE_ERROR)
		rotate_state=RIGHT;
	else if(RVals.LRx<primary_value_LRx-REMOTE_ERROR)
			rotate_state=LEFT;
	else
		rotate_state=STRAIGHT;
}

void left_forward(int duty_cycle)
{
	DUTY1=duty_cycle;
	IN1=1,IN2=0;
}

void right_forward(int duty_cycle)
{
	DUTY2=duty_cycle;
	IN3=1,IN4=0;
}

void forward(int duty_cycle)
{
	left_forward(duty_cycle);
	right_forward(duty_cycle);
}

void left_backward(int duty_cycle)
{
	DUTY1=duty_cycle;
	IN1=0,IN2=1;
}

void right_backward(int duty_cycle)
{
	DUTY2=duty_cycle;
	IN3=0,IN4=1;
}

void backward(int duty_cycle)
{
	left_backward(duty_cycle);
	right_backward(duty_cycle);
}

void rotate_left(int duty_cycle)//只旋转
{
	DUTY1=DUTY2=round(duty_cycle);	
	IN1=0,IN2=1;
	IN3=1,IN4=0;
//	DUTY1=DUTY2=duty_cycle;	
//	IN1=1,IN2=0;
//	IN3=0,IN4=1;
}
void rotate_right(int duty_cycle)
{
	DUTY1=DUTY2=round(duty_cycle);
	IN1=1,IN2=0;
	IN3=0,IN4=1;
	
	
	
	
	
	
	
//	DUTY1=DUTY2=duty_cycle;	
//	IN1=0,IN2=1;
//	IN3=1,IN4=0;
}

void rotate_go_left(int duty_cycle,int d_duty_cycle)//d_duty_cycle越大旋转越快前进越慢
{
	DUTY2=duty_cycle;	
	DUTY1=duty_cycle-d_duty_cycle;
	IN1=1,IN2=0;
	IN3=1,IN4=0;
}
void rotate_go_right(int duty_cycle,int d_duty_cycle)
{
	DUTY1=duty_cycle;	
	DUTY2=duty_cycle-d_duty_cycle;
	IN1=1,IN2=0;
	IN3=1,IN4=0;
}

