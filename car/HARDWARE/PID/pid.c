/**
  ******************************************************************************
  * @file    pid.c 
  * @author  Tang Zhixuan
  * @version V1.0
  * @date    2017/5/13
  ******************************************************************************
  */

#include "pid.h"
extern float target_move_speed;//��ȡ��ң������ֵ
float left_speed_dynamic,right_speed_dynamic;
float left_duty_cycle_cal,right_duty_cycle_cal;//����������ֵ�ռ�ձ�
PID_Type* PID_left;
PID_Type* PID_right;

void set_default_value(void)
{
PID_left->kp=KP_DEFAULT;
PID_left->ki=KI_DEFAULT;
PID_left->kd=KD_DEFAULT;
PID_right->kp=KP_DEFAULT;
PID_right->ki=KI_DEFAULT;
PID_right->kd=KD_DEFAULT;
}

void pid_cal(PID_Type* PID)
{
  float dErrP, dErrI, dErrD;
  dErrP = PID->errNow - PID->errOld1;
  dErrI = PID->errNow;
  dErrD = PID->errNow - 2.0f * PID->errOld1 + PID->errOld2;
  PID->errOld2 = PID->errOld1; 
  PID->errOld1 = PID->errNow;  
  PID->doutput = PID->kp * dErrP + PID->ki * dErrI + PID->kd * dErrD;
  PID->output += PID->output;//ռ�ձ�
}


void speed_process(void)
{
   PID_left->errNow = target_move_speed - left_speed_dynamic; //���㲢д���ٶ����
   pid_cal(PID_left);   
	 if(left_duty_cycle_cal>DUTY_MAX) left_duty_cycle_cal=DUTY_MAX;	
   else{left_duty_cycle_cal = PID_left->output;} //�����outputΪռ�ձ�
   PID_right->errNow = target_move_speed - right_speed_dynamic; //���㲢д���ٶ����
   pid_cal(PID_right);  
	 if(right_duty_cycle_cal>DUTY_MAX) right_duty_cycle_cal=DUTY_MAX;		 
   else{right_duty_cycle_cal = PID_right->output;} //�����outputΪռ�ձ�
   move();        //����PWM
}
