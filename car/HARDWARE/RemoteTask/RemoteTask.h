#ifndef REMOTETASK_H
#define REMOTETASK_H

#include "sys.h"
#include "SPI.h"
#include "delay.h"
#include "24l01.h"

typedef __packed struct
{
	u16 LRx;
	u16 LRy;
	u16 RRx;
	u16 RRy;
	u8 KLU;
	u8 KLD;
	u8 KRU;
	u8 KRD;
} Rstruct;

void Remote_Init(void);
void Remote_Receive(Rstruct *RVals);
void Remote_Encode(Rstruct RVals);
void Remote_Decode(Rstruct *RVals);


#endif
