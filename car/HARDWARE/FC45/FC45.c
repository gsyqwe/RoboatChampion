#include "FC45.h"
//���м����or��ͬʱ̽�⵽��˵�����ڼ���սǣ�������STATE000
//Ȼ����������ҵ���
void line_following()
{
	while(1)
	{
		forward(LINE_FOLLOWING_DUTY);
		if(STATE001)//����м���ս�
		{
			forward(LINE_FOLLOWING_DUTY + ADD_DUTY);
			while(!STATE000);
      rotate_left(LINE_FOLLOWING_DUTY);
			while(TTL1!=0);		
		}
		if(STATE100)//�ұ��м���ս�
		{
			forward(LINE_FOLLOWING_DUTY + ADD_DUTY);
			while(!STATE000);
      rotate_right(LINE_FOLLOWING_DUTY);
			while(TTL3!=0);		
		}
		while(STATE011)//ֻ����߱�����
		{
			rotate_go_left(LINE_FOLLOWING_DUTY, D_DUTY);
		}
		while(STATE110)
		{
			rotate_go_right(LINE_FOLLOWING_DUTY, D_DUTY);
		}

	}
}
