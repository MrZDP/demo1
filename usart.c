#include <stm32f10x.h>
#include "stdio.h"
#include "stdlib.h"

void USART2_SendStr(u8 *str);
void USART2_SendByte(u8 data);
void USART2_init(u32 baud);
void USART2_pin_init(void);

//USART1���ų�ʼ��
//USART1_TX(PA9) , USART1_RX(PA10)
void USART1_pin_init(void)
{
	//����GPIO���Ľṹ�����
	GPIO_InitTypeDef gpio_InitStruct;
	//����GPIOA��ʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
	//�Ƚ�PA9����Ϊ���츴�����
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_9 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ; 
	GPIO_Init (GPIOA, &gpio_InitStruct);
	//�ٽ�PA10����Ϊ��������
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_10 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ; 
	GPIO_Init (GPIOA, &gpio_InitStruct);
}

//USART1��ʼ��
void USART1_init(u32 baud)
{
	//����USART���Ľṹ�����
	USART_InitTypeDef usart_InitStruct;
	
	//USART1���ų�ʼ��
	USART1_pin_init();
	
	//����USART1��ʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1, ENABLE);
	
	//USART1��ͨ��Э������
	usart_InitStruct.USART_BaudRate = baud ;
	usart_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
	usart_InitStruct.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx ;
	usart_InitStruct.USART_Parity = USART_Parity_No ;
	usart_InitStruct.USART_StopBits = USART_StopBits_1 ;
	usart_InitStruct.USART_WordLength = USART_WordLength_8b ;

	USART_Init (USART1, &usart_InitStruct);
	
	//USART1��ʹ��
	USART_Cmd (USART1, ENABLE);
}

//USART1���͵��ֽ�
void USART1_SendByte(u8 data)
{
	//�ȴ��ϴη������
	while(USART_GetFlagStatus (USART1, USART_FLAG_TC) != SET);
	//׼�����η��͵�����
	USART_SendData (USART1, data);
	//�ȴ����η������
	while(USART_GetFlagStatus (USART1, USART_FLAG_TC) != SET);
}

//USART1�����ַ���
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
	//����GPIO���Ľṹ�����
	GPIO_InitTypeDef gpio_InitStruct;
	//����GPIOA��ʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
	//�Ƚ�PA9����Ϊ���츴�����
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_2 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ; 
	GPIO_Init (GPIOA, &gpio_InitStruct);
	//�ٽ�PA10����Ϊ��������
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_3 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ; 
	GPIO_Init (GPIOA, &gpio_InitStruct);
}

//USART1��ʼ��
void USART2_init(u32 baud)
{
	//����USART���Ľṹ�����
	USART_InitTypeDef usart_InitStruct;
	
	//USART1���ų�ʼ��
	USART1_pin_init();
	
	//����USART1��ʱ��
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2, ENABLE);
	
	//USART1��ͨ��Э������
	usart_InitStruct.USART_BaudRate = baud ;
	usart_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
	usart_InitStruct.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx ;
	usart_InitStruct.USART_Parity = USART_Parity_No ;
	usart_InitStruct.USART_StopBits = USART_StopBits_1 ;
	usart_InitStruct.USART_WordLength = USART_WordLength_8b ;

	USART_Init (USART2, &usart_InitStruct);
	
	//USART1��ʹ��
	USART_Cmd (USART2, ENABLE);
}

//USART1���͵��ֽ�
void USART2_SendByte(u8 data)
{
	//�ȴ��ϴη������
	while(USART_GetFlagStatus (USART2, USART_FLAG_TC) != SET);
	//׼�����η��͵�����
	USART_SendData (USART2, data);
	//�ȴ����η������
	while(USART_GetFlagStatus (USART2, USART_FLAG_TC) != SET);
}

//USART1�����ַ���
void USART2_SendStr(u8 *str)
{
	while(*str)
	{
		USART2_SendByte(*str);
		str++;
	}
	USART2_SendByte(*str);
}


/*******************************printf����ض���*********************************/


#pragma import(__use_no_semihosting_swi) //ȡ��������״̬

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) 
{
	//�滻Ϊ�������ض���ķ��͵��ֽں���
	USART1_SendByte(ch);
  return (ch);
}

int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  //�滻Ϊ�������ض���ķ��͵��ֽں���
	USART1_SendByte(ch);
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
