#ifndef __FC45_H
#define __FC45_H
#include "gpio.h"
#include "pwm.h"

#define STATE111 (TTL1==1&&TTL2==1&&TTL3==1) //1ÎªÎ´´¥·¢£¨Î´¼ì²âµ½ºÚÏß
#define STATE011 (TTL1==0&&TTL2==1&&TTL3==1) //×ó²à´¥·¢
#define STATE101 (TTL1==1&&TTL2==0&&TTL3==1)
#define STATE110 (TTL1==1&&TTL2==1&&TTL3==0)
#define STATE100 (TTL1==1&&TTL2==0&&TTL3==0)
#define STATE010 (TTL1==0&&TTL2==1&&TTL3==0)
#define STATE001 (TTL1==0&&TTL2==0&&TTL3==1)
#define STATE000 (TTL1==0&&TTL2==0&&TTL3==0) 
#define LINE_FOLLOWING_DUTY 18000
#define ADD_DUTY 5000
#define D_DUTY 0.8*LINE_FOLLOWING_DUTY

void line_following(void);

#endif
