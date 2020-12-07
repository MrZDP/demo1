#include <stm32f10x.h>
#include "step_motor.h"

//��������ת+A ? +B ? -A ? -B
u8 single4_cw[4] = {0x01,0x04,0x02,0x08};
//�����ķ�ת
u8 single4_ccw[4] = {0x08,0x02,0x04,0x01};

//˫������ת+A+B ? +B-A ? -A-B ? -B+A ? +A+B
u8 double4_cw[4] = {0x05,0x06,0x0A,0x09};
//˫���ķ�ת
u8 double4_ccw[4] = {0x09,0x0A,0x06,0x05};

//���������ʼ��
//IN1 -- PA7 -- +A 
//IN2 -- PA6 -- -A 
//IN3 -- PA5 -- +B 
//IN4 -- PA4 -- -B 
void Step_Motor_init(void)
{
	/*��PA4~PA7����Ϊ�����������ʼ����͵�ƽ*/
	//����GPIO���Ľṹ�����
	GPIO_InitTypeDef gpio_InitStruct;
	
	//����GPIOAʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA,ENABLE);   

	//�ֱ�����PA7~4�������
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOA, &gpio_InitStruct);
	
	//��ʼʱ��IN1~4�͵�ƽ
	GPIO_ResetBits (GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}

//���������ת(ת��ת��һȦ)
//˫����
void Step_Motor_cw(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		if(double4_cw[i] & (1<<0))
		{
			Step_Motor_IN1_H;
		}
		else
		{
			Step_Motor_IN1_L;
		}
		if(double4_cw[i] & (1<<1))
		{
			Step_Motor_IN2_H;
		}
		else
		{
			Step_Motor_IN2_L;
		}
		if(double4_cw[i] & (1<<2))
		{
			Step_Motor_IN3_H;
		}
		else
		{
			Step_Motor_IN3_L;
		}
		if(double4_cw[i] & (1<<3))
		{
			Step_Motor_IN4_H;
		}
		else
		{
			Step_Motor_IN4_L;
		}
		SysTick_Delayms(5);
	}
}

//���������ת(ת��ת��һȦ)
//˫����
void Step_Motor_ccw(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		if(double4_ccw[i] & (1<<0))
		{
			Step_Motor_IN1_H;
		}
		else
		{
			Step_Motor_IN1_L;
		}
		if(double4_ccw[i] & (1<<1))
		{
			Step_Motor_IN2_H;
		}
		else
		{
			Step_Motor_IN2_L;
		}
		if(double4_ccw[i] & (1<<2))
		{
			Step_Motor_IN3_H;
		}
		else
		{
			Step_Motor_IN3_L;
		}
		if(double4_ccw[i] & (1<<3))
		{
			Step_Motor_IN4_H;
		}
		else
		{
			Step_Motor_IN4_L;
		}
		SysTick_Delayms(5);
	}
}

//���һ���������ڽ���������ƶ����ض���λ�ã�������˿������4���㣩

