#include <stm32f10x.h>

//LED1 �C PB11
//LED2 �C PB14
//LED3 �C PA11
void LED_init(void)
{
	//����GPIO���Ľṹ�����
	GPIO_InitTypeDef gpio_InitStruct;
	
	//����GPIOA��GPIOBʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);   

	//�ֱ�����PB11��PB14��PA11�������
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOB, &gpio_InitStruct);
	
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_11 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOA, &gpio_InitStruct);
	
	//��ʼʱ��LEDϨ��
	GPIO_ResetBits (GPIOB, GPIO_Pin_11);
	GPIO_ResetBits (GPIOB, GPIO_Pin_14);
	GPIO_ResetBits (GPIOA, GPIO_Pin_11);
}

