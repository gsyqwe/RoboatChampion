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

extern Rstruct RVals;//����ң���������ϸ���
int move_speed;
int rotate_speed;//ռ�ձ�
int move_state,rotate_state;
extern float left_duty_cycle_cal,right_duty_cycle_cal;//����������ֵ�ռ�ձ�
int primary_value_LRx,primary_value_RRy;

void TIM1_PWM_Init(u16 arr,u16 psc)//PWM Ƶ��=72000/(arr+1)KHz,ȡarr=35999��psc=1��PWMƵ��1kHz
{		 		
  GPIO_InitTypeDef  GPIO_InitStructure;	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOE,&GPIO_InitStructure);              
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ,0
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=0; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��1
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ PWM1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
  DUTY1=0;
	DUTY2=0;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� 
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
 	TIM_CtrlPWMOutputs(TIM1,ENABLE); //�߼���ʱ��Ҫ��
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

//��ת����û�н���pid����
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

//������
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

void rotate_left(int duty_cycle)//ֻ��ת
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

void rotate_go_left(int duty_cycle,int d_duty_cycle)//d_duty_cycleԽ����תԽ��ǰ��Խ��
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

