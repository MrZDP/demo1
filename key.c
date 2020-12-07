#include <stm32f10x.h>

//KEY1 �C PA0   (����ʶ��ߵ�ƽ)
//KEY2 �C PA8   (����ʶ��͵�ƽ)
//KEY3 �C PB10  (����ʶ��͵�ƽ)
void KEY_init(void)
{
	//����GPIO���Ľṹ�����
	GPIO_InitTypeDef gpio_InitStruct;
	
	//����GPIOA��GPIOBʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);   

	//�ֱ�����PA0��PA8��PB10 ��������
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING  ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOA, &gpio_InitStruct);
	
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_10 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOB, &gpio_InitStruct);
	
}