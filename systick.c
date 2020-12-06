#include <stm32f10x.h>

//�δ�ʱ�����뼶��ʱ���������ʱ1.86s
void SysTick_Delayms(u32 xms)
{
	//ѡ��ʱ��Դ��ѡ�����Ƶ�ʣ�:9MHz
	SysTick->CTRL &=~(1<<2);
	//���ü�������
	SysTick->LOAD = 9000*xms;
	//�򿪼�����
	SysTick->CTRL |= (1<<0);
	//�ȴ��������
	while( !(SysTick->CTRL & (1<<16)) );
	//�رռ�����
	SysTick->CTRL &=~(1<<0);
}

//�δ�ʱ��΢�뼶��ʱ���������ʱ1.86s
void SysTick_Delayus(u32 xus)
{
	//ѡ��ʱ��Դ��ѡ�����Ƶ�ʣ�:9MHz
	SysTick->CTRL &=~(1<<2);
	//���ü�������
	SysTick->LOAD = 9*xus;
	//�򿪼�����
	SysTick->CTRL |= (1<<0);
	//�ȴ��������
	while( !(SysTick->CTRL & (1<<16)) );
	//�رռ�����
	SysTick->CTRL &=~(1<<0);
}