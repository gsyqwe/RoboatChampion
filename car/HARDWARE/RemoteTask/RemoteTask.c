#include "RemoteTask.h"
/**
  ******************************************************************************
  * @author  WuPeiCong
  * @version V1.56
  * @date    2016-05-7
	******************************************************************************
*/
Rstruct RVals={0,0,0,0,0,0,0,0};
u8 data[32];
char calibration =0;
TIM_ICInitTypeDef  TIM4_ICInitStructure;
extern int primary_value_LRx,primary_value_RRy;

void Remote_Init(){
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	NRF24L01_Init();
	while(NRF24L01_Check());
	NRF24L01_RX_Mode();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =71; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM2���벶�����
	TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//�����ز���
  TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM3, &TIM4_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��4
}

void Remote_Receive(Rstruct *RVals){
	NRF24L01_RxPacket(data);
	Remote_Decode(RVals);
}
void Remote_Encode(Rstruct RVals){	
	data[0]=RVals.LRx%0x100;
	data[1]=RVals.LRx/0x100;
	data[2]=RVals.LRy%0x100;
	data[3]=RVals.LRy/0x100;
	data[4]=RVals.RRx%0x100;
	data[5]=RVals.RRx/0x100;
	data[6]=RVals.RRy%0x100;
	data[7]=RVals.RRy/0x100;
	data[8]=RVals.KLD+RVals.KLU*0x02+RVals.KRD*0x04+RVals.KRU*0x08;
}

void Remote_Decode(Rstruct *RVals){
	RVals->LRx=data[0]+data[1]*0x100;
	RVals->LRy=data[2]+data[3]*0x100;
	RVals->RRx=data[4]+data[5]*0x100;
	RVals->RRy=data[6]+data[7]*0x100;
	RVals->KLD=data[8]&0x01;
	RVals->KLU=(data[8]&0x02)<<1;
	RVals->KRD=(data[8]&0x04)<<2;
	RVals->KRU=(data[8]&0x08)<<3;
}

void TIM4_IRQHandler(void){
	u8 sta;
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update);
	sta=NRF24L01_Read_Reg(STATUS);
	if(sta&RX_OK){
		Remote_Receive(&RVals);
		if(calibration==0){
			calibration=1;
			primary_value_LRx=RVals.LRx;
			primary_value_RRy=RVals.RRy;
		}
	}
}