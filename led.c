#include <stm32f10x.h>

//LED1 – PB11
//LED2 – PB14
//LED3 – PA11
void LED_init(void)
{
	//定义GPIO核心结构体变量
	GPIO_InitTypeDef gpio_InitStruct;
	
	//激活GPIOA和GPIOB时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);   

	//分别配置PB11、PB14、PA11推挽输出
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOB, &gpio_InitStruct);
	
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_11 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOA, &gpio_InitStruct);
	
	//初始时，LED熄灭
	GPIO_ResetBits (GPIOB, GPIO_Pin_11);
	GPIO_ResetBits (GPIOB, GPIO_Pin_14);
	GPIO_ResetBits (GPIOA, GPIO_Pin_11);
}

