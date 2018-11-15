/**
  ******************************************************************************
  * @file    pwm.h
  * @author  Tang Zhixuan
  * @version V1.0
  * @date    2017/5/13
  ******************************************************************************
  */
#ifndef __PWM_H
#define __PWM_H
#include "stdint.h"
#include "sys.h"
#include "math.h"
#include "RemoteTask.h"

#define TIM1_CH1_GPIO PEout(9)//wheel1
#define TIM1_CH2_GPIO PEout(11)//wheel2
#define DUTY1 TIM1->CCR1
#define DUTY2 TIM1->CCR2
#define ARR 35999
#define DUTY_MAX (ARR+1)
#define MAX_SPEED 10//rad/s,根据实际情况来改
#define SPEED_1 MAX_SPEED/4
#define SPEED_2 MAX_SPEED/2
#define SPEED_3 MAX_SPEED*3/4
#define FORWARD 0
#define BACKWARD 1
#define STOP 2
#define LEFT 0
#define RIGHT 1
#define STRAIGHT 2
#define REMOTE_ERROR 10

//#define ERROR_STOP 50

void TIM1_PWM_Init(u16 arr,u16 psc);//arr==PERIOD
void move(void);
void get_speed_state(void);
void left_forward(int duty_cycle);
void right_forward(int duty_cycle);
void left_backward(int duty_cycle);
void right_backward(int duty_cycle);
void rotate_left(int duty_cycle);//只旋转
void rotate_right(int duty_cycle);
void rotate_go_left(int duty_cycle,int d_duty_cycle);//边拐弯边前进
void rotate_go_right(int duty_cycle,int d_duty_cycle);
void forward(int duty_cycle);
void backward(int duty_cycle);


#endif
