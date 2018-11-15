#include "main.h"

extern Rstruct RVals;
extern char calibration;
int main(void)
{
	u8 sta=0;
	delay_init(84);
	gpio_init();
	IN1=IN2=IN3=IN4=0;
  TIM1_PWM_Init(ARR,1);
//	TIM2_Init();
	Remote_Init();
	while(!calibration);
	while(1){
	  get_speed_state();
		move();
	}
}
