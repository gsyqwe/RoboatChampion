/**
  ******************************************************************************
  * @file    pid.h
  * @author  Tang Zhixuan
  * @version V1.0
  * @date    2017/5/13
  ******************************************************************************
  */

#ifndef __PID_H
#define __PID_H
#include "pwm.h"

#define KP_DEFAULT 100.0f
#define KI_DEFAULT 0.0f
#define KD_DEFAULT 0.0f

//����ʽPID�㷨
typedef struct 
{
 float kp;     
 float ki;     
 float kd;//��������Ĭ��ֵ����define�� Ϊ1��0��0. �����е���ȡ�ʺϵ�����
 float errNow; //��ǰ�����
 float errOld1;
 float errOld2;
 float doutput;//�����������
 int  output;//�������
}PID_Type;

void set_default_value(void);
void pid_cal(PID_Type* PID);
void speed_process(void);
	
#endif
