#include <stm32f10x.h>

//TIM1���뼶��ʱ
//����Ƶ�ʣ�72MHz
//  arr/(72M/psc) == xms/1000
// arr*psc == xms*72000
void TIM1_Delayms(u32 xms)
{
	//����TIM���Ľṹ�����
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitStruct;
	
	//����TIM1��ʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1, ENABLE);
	
	//����TIM1��ʱ����Ԫ
	tim_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1 ;
	tim_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up ;
	tim_TimeBaseInitStruct.TIM_Period = 10*xms - 1 ; //�´θ����¼�����װ�ؼĴ���ֵ
	tim_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1 ; //Ԥ��Ƶ�Ĵ���ֵ
	tim_TimeBaseInitStruct.TIM_RepetitionCounter = 0 ;

	TIM_TimeBaseInit (TIM1, &tim_TimeBaseInitStruct);
	
	//���������¼�������ARRֵ��
	TIM_GenerateEvent (TIM1, TIM_EventSource_Update);
	TIM_ClearFlag (TIM1, TIM_FLAG_Update);
	
	//����������
	TIM_SetCounter (TIM1, 0);
	
	//TIM1��������
	TIM_Cmd (TIM1, ENABLE);
	
	//�ȴ�������ɣ�ֹͣ�����������־λ
	while(TIM_GetFlagStatus (TIM1, TIM_FLAG_Update) != SET);
	TIM_Cmd (TIM1, DISABLE);
	TIM_ClearFlag (TIM1, TIM_FLAG_Update);
}
