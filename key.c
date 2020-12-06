#include <stm32f10x.h>

//KEY1 – PA0   (按下识别高电平)
//KEY2 – PA8   (按下识别低电平)
//KEY3 – PB10  (按下识别低电平)
void KEY_init(void)
{
	//定义GPIO核心结构体变量
	GPIO_InitTypeDef gpio_InitStruct;
	
	//激活GPIOA和GPIOB时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);   

	//分别配置PA0、PA8、PB10 浮空输入
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING  ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOA, &gpio_InitStruct);
	
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_10 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOB, &gpio_InitStruct);
	
}