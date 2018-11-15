/**
  ******************************************************************************
  * @file    timer.c 
  * @author  Tang Zhixuan
  * @version V1.0
  * @date    2017/5/13
  ******************************************************************************
  */

#include "timer.h"
int encoder_state = 0;
int leftcount=0,rightcount=0;
int leftstate,rightstate;
int TIM2count;
extern float left_speed_dynamic,right_speed_dynamic;
extern PID_Type* PID_left;
extern PID_Type*	PID_right;
extern Rstruct RVals;


//������ת��240r/min����4r/s�����̹�20��0.0125s/��,80��/s����TIM2�ж����ڣ�
//TIM2 ��ʼ42MHz
//���Ϊÿ0.001s�ж�һ�Σ�ÿ200�δ���һ������,f=1kHz
void TIM2_Init(void)//TIM4�ж����ڶ�ȡ�ٶ�,����error
{
     TIM_TimeBaseInitTypeDef  tim;
		NVIC_InitTypeDef nvic;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    tim.TIM_Prescaler = (84-1);        //168/84=2MHz
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 2000;//1KHz
    TIM_TimeBaseInit(TIM2,&tim);
	  TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM2, ENABLE);	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
   nvic.NVIC_IRQChannel = TIM2_IRQn;
   nvic.NVIC_IRQChannelPreemptionPriority = 0x00;
   nvic.NVIC_IRQChannelSubPriority = 0x03;
   nvic.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&nvic);
}

void TIM2_IRQHandler(void)  //��ȡ�ٶ���pid����
{
    if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET && RVals.KLU!=1) 
	  {
			if(encoder_state==0)//С�����ϵ磬�ȴ�����ٶ��ȶ�
			{
				encoder_state=1;	//�տ�ʼ����			
				delay_ms(4000);
			}
			else
			{
				TIM2count++;//�жϴ�����ʼ����
				if(encoder_state==1)//��һ�μ�����ֻ��¼
				{
				  leftstate=LEFT_INPUT;
				  rightstate=RIGHT_INPUT;
				  encoder_state=2;
				}
				else
				{
					if(TIM2count==70)
					{
						left_speed_dynamic=leftcount/(TIM2_ITPERIOD*70*40);//rad/s
						right_speed_dynamic=rightcount/(TIM2_ITPERIOD*70*40);				
						TIM2count=0;
						leftcount=0;
						rightcount=0;
						speed_process();
					}
					else{
				    if(LEFT_INPUT!=leftstate)
				    {
				    	leftcount++;
				    	leftstate=LEFT_INPUT;
			  	  }
			    	if(RIGHT_INPUT!=leftstate)
			    	{
				    	rightcount++;
				    	rightstate=RIGHT_INPUT;
				    }
				  }
			  }
			}
		}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}


