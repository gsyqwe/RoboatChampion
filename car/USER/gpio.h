#ifndef __GPIO_H
#define __GPIO_H
#include "sys.h"
//E9,E11 PWM

#define IN1 PBout(15)
#define IN2 PDout(8)
#define IN3 PBout(14)
#define IN4 PBout(13)
//L298N

#define LEFT_INPUT PAin(1)
#define RIGHT_INPUT PAin(2)
//get speed

//#define TTL1 PEin(0)
//#define TTL2 PEin(1)
//#define TTL3 PEin(2)
//循线传感器,左中右

#define NRF24L01_CE   PBout(6) 	//24L01片选信号
#define NRF24L01_CSN  PBout(7) 	//SPI片选信号	   
#define NRF24L01_IRQ  PBin(8)  	//IRQ主机数据输入
//24L01操作线

//PB3-5被spi占用

void gpio_init(void);
#endif
