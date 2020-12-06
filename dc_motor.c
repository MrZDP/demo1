#include <stm32f10x.h>

#define CNTMAX	1000
//ֱ�������ʼ������
//IA--PB5--TIM3_CH2
//IB--PB4--TIM3_CH1
//�ȹ涨PWM���� 10ms
void DC_Motor_init(void)
{
	/*1. ��PB4\PB5����ΪTIM3_CH1\CH2*/
	
	//����GPIO���Ľṹ�����
	GPIO_InitTypeDef gpio_InitStruct;
	//����TIM���Ľṹ�����
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitStruct;
	//����TIMOC���Ľṹ�����
	TIM_OCInitTypeDef  tim_OCInitStruct;   

	
	//����GPIOB��AFIOʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	
	//ȡ��SWJ��JTRST��ӳ��
	GPIO_PinRemapConfig (GPIO_Remap_SWJ_NoJTRST, ENABLE);
	//ʵ��TIM3�Ĳ���ӳ�䣨��֤PB4��PB5���Ը���ΪTIM3��ͨ�����ţ�
	GPIO_PinRemapConfig (GPIO_PartialRemap_TIM3, ENABLE);
	
	//��PB4��PB5����TIM3��CH2��CH1�����츴�����
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOB, &gpio_InitStruct);
	
	/*2. TIM3ʱ����Ԫ����*/
	//����TIM3��ʱ��
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
	
	//����TIM3��ʱ����Ԫ
	tim_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1 ;
	tim_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up ;
	tim_TimeBaseInitStruct.TIM_Period = CNTMAX - 1; //�´θ����¼�����װ�ؼĴ���ֵ
	tim_TimeBaseInitStruct.TIM_Prescaler = 720 - 1 ; //Ԥ��Ƶ�Ĵ���ֵ
	tim_TimeBaseInitStruct.TIM_RepetitionCounter = 0 ;

	TIM_TimeBaseInit (TIM3, &tim_TimeBaseInitStruct);
	
	/*3. TIM3OC1��OC2����*/
	tim_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1 ;
	tim_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High ;
	tim_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable ;
	tim_OCInitStruct.TIM_Pulse = 0 ;
	
	TIM_OC1Init (TIM3, &tim_OCInitStruct);
	TIM_OC2Init (TIM3, &tim_OCInitStruct);
	
	
	/*4. ������ʱ�� */
	TIM_Cmd (TIM3, ENABLE);
}
	

//ֱ�����ת������
//������int v_percent ��ʾ�ٶȵİٷֱȣ���Χ-100~+100
//ռ�ձ� = CCR / ARR =  |v_percent| / 100
void DC_Motor_Run(int v_percent)
{
		if(v_percent >= 0)
		{
			//��ת
			TIM_SetCompare2 (TIM3, v_percent * CNTMAX / 100);
			TIM_SetCompare1 (TIM3, 0);
		}
		else
		{
			//��ת
			TIM_SetCompare2 (TIM3, 0);
			TIM_SetCompare1 (TIM3, v_percent * CNTMAX / (-100));
		}
}



