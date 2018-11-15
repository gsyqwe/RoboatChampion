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

//增量式PID算法
typedef struct 
{
 float kp;     
 float ki;     
 float kd;//三个参数默认值（见define） 为1，0，0. 请自行调参取适合的数字
 float errNow; //当前的误差
 float errOld1;
 float errOld2;
 float doutput;//控制增量输出
 int  output;//控制输出
}PID_Type;

void set_default_value(void);
void pid_cal(PID_Type* PID);
void speed_process(void);
	
#endif
