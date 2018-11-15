#include "FC45.h"
//若中间和左or右同时探测到，说明存在尖锐拐角，加速至STATE000
//然后拐弯重新找到线
void line_following()
{
	while(1)
	{
		forward(LINE_FOLLOWING_DUTY);
		if(STATE001)//左边有尖锐拐角
		{
			forward(LINE_FOLLOWING_DUTY + ADD_DUTY);
			while(!STATE000);
      rotate_left(LINE_FOLLOWING_DUTY);
			while(TTL1!=0);		
		}
		if(STATE100)//右边有尖锐拐角
		{
			forward(LINE_FOLLOWING_DUTY + ADD_DUTY);
			while(!STATE000);
      rotate_right(LINE_FOLLOWING_DUTY);
			while(TTL3!=0);		
		}
		while(STATE011)//只有左边被触发
		{
			rotate_go_left(LINE_FOLLOWING_DUTY, D_DUTY);
		}
		while(STATE110)
		{
			rotate_go_right(LINE_FOLLOWING_DUTY, D_DUTY);
		}

	}
}
