#include <stm32f10x.h>
#include "stdio.h"
#include "stdlib.h"

void USART2_SendStr(u8 *str);
void USART2_SendByte(u8 data);
void USART2_init(u32 baud);
void USART2_pin_init(void);

//USART1引脚初始化
//USART1_TX(PA9) , USART1_RX(PA10)
void USART1_pin_init(void)
{
	//定义GPIO核心结构体变量
	GPIO_InitTypeDef gpio_InitStruct;
	//激活GPIOA的时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
	//先将PA9配置为推挽复用输出
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_9 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ; 
	GPIO_Init (GPIOA, &gpio_InitStruct);
	//再将PA10配置为浮空输入
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_10 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ; 
	GPIO_Init (GPIOA, &gpio_InitStruct);
}

//USART1初始化
void USART1_init(u32 baud)
{
	//定义USART核心结构体变量
	USART_InitTypeDef usart_InitStruct;
	
	//USART1引脚初始化
	USART1_pin_init();
	
	//激活USART1的时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
	
	//USART1的通信协议配置
	usart_InitStruct.USART_BaudRate = baud ;
	usart_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
	usart_InitStruct.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx ;
	usart_InitStruct.USART_Parity = USART_Parity_No ;
	usart_InitStruct.USART_StopBits = USART_StopBits_1 ;
	usart_InitStruct.USART_WordLength = USART_WordLength_8b ;

	USART_Init (USART1, &usart_InitStruct);
	
	//USART1的使能
	USART_Cmd (USART1, ENABLE);
}

//USART1发送单字节
void USART1_SendByte(u8 data)
{
	//等待上次发送完毕
	while(USART_GetFlagStatus (USART1, USART_FLAG_TC) != SET);
	//准备本次发送的数据
	USART_SendData (USART1, data);
	//等待本次发送完毕
	while(USART_GetFlagStatus (USART1, USART_FLAG_TC) != SET);
}

//USART1发送字符串
void USART1_SendStr(u8 *str)
{
	while(*str)
	{
		USART1_SendByte(*str);
		str++;
	}
	USART1_SendByte(*str);
}


/************************************USART2****************************************/
void USART2_pin_init(void)
{
	//定义GPIO核心结构体变量
	GPIO_InitTypeDef gpio_InitStruct;
	//激活GPIOA的时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
	//先将PA9配置为推挽复用输出
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_2 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ; 
	GPIO_Init (GPIOA, &gpio_InitStruct);
	//再将PA10配置为浮空输入
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_3 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ; 
	GPIO_Init (GPIOA, &gpio_InitStruct);
}

//USART1初始化
void USART2_init(u32 baud)
{
	//定义USART核心结构体变量
	USART_InitTypeDef usart_InitStruct;
	
	//USART1引脚初始化
	USART1_pin_init();
	
	//激活USART1的时钟
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2, ENABLE);
	
	//USART1的通信协议配置
	usart_InitStruct.USART_BaudRate = baud ;
	usart_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
	usart_InitStruct.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx ;
	usart_InitStruct.USART_Parity = USART_Parity_No ;
	usart_InitStruct.USART_StopBits = USART_StopBits_1 ;
	usart_InitStruct.USART_WordLength = USART_WordLength_8b ;

	USART_Init (USART2, &usart_InitStruct);
	
	//USART1的使能
	USART_Cmd (USART2, ENABLE);
}

//USART1发送单字节
void USART2_SendByte(u8 data)
{
	//等待上次发送完毕
	while(USART_GetFlagStatus (USART2, USART_FLAG_TC) != SET);
	//准备本次发送的数据
	USART_SendData (USART2, data);
	//等待本次发送完毕
	while(USART_GetFlagStatus (USART2, USART_FLAG_TC) != SET);
}

//USART1发送字符串
void USART2_SendStr(u8 *str)
{
	while(*str)
	{
		USART2_SendByte(*str);
		str++;
	}
	USART2_SendByte(*str);
}


/*******************************printf输出重定向*********************************/


#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
	//替换为开发者重定向的发送单字节函数
	USART1_SendByte(ch);
  return (ch);
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  //替换为开发者重定向的发送单字节函数
	USART1_SendByte(ch);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
