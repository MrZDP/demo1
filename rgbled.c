#include <stm32f10x.h>
#include "rgbled.h"

//RGB���ų�ʼ��
//RGBDATA -- PB9
//RGBCLK -- PB8
void RGBLED_init(void)
{
	//����GPIO���Ľṹ�����
	GPIO_InitTypeDef gpio_InitStruct;
	
	//����GPIOBʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);   

	//�ֱ�����PB8��PB9�������
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOB, &gpio_InitStruct);

	//��ʼʱCLK��DATA����
	GPIO_ResetBits (GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
}

//��RGBд��u32���ݣ���λ��ǰ��MSB��
void RGBLED_write(u32 data)
{
	int i;
	//����ʱCLK����
	CLK_L;
	//����32λ����
	for(i=31;i>=0;i--) //ʹ�ø���ʽʱȷ������i�� �з�������
	{
		//ʱ�������ߣ�׼������
		CLK_H;
		if(data & (1<<i))
		{
			DATA_H;
		}
		else
		{
			DATA_L;
		}
		//ʱ����������ʱ
		SysTick_Delayus(5);
		
		//ʱ�������ͣ������½��أ�����һλ����
		CLK_L;
		//ʱ����������ʱ
		SysTick_Delayus(5);
	}

	//����ʱCLK����
	CLK_L;
}

//RGB��ʾ��ɫ
//������u32 color ����R[23:16],G[15:8],B[7:0]��ʽ
void RGBLED_Display(u32 color)
{
	u32 data = 0;
	u8 r,g,b;
	r = (u8)(color >> 16);
	g = (u8)(color >> 8);
	b = (u8)(color >> 0);
	//����־λ
	data |= (0x3 << 30);
	//���У����
	data |= ((b ^ 0xC0)>>6) << 28;
	data |= ((g ^ 0xC0)>>6) << 26;
	data |= ((r ^ 0xC0)>>6) << 24;
	data |= (b << 16);
	data |= (g << 8);
	data |= (r << 0);
	//������ʼ֡
	RGBLED_write(0);
	//�����һ��������
	RGBLED_write(data);
}
