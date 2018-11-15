/**
  ******************************************************************************
  * @file    timer.h 
  * @author  Tang Zhixuan
  * @version V1.0
  * @date    2017/5/13
  ******************************************************************************
  */

#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "delay.h"
#include "pwm.h"
#include "pid.h"
#include "speed.h"
#include "gpio.h"

#define TIM2_ITPERIOD 0.001f
void TIM2_Init(void);

#endif
