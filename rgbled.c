#include <stm32f10x.h>
#include "rgbled.h"

//RGB引脚初始化
//RGBDATA -- PB9
//RGBCLK -- PB8
void RGBLED_init(void)
{
	//定义GPIO核心结构体变量
	GPIO_InitTypeDef gpio_InitStruct;
	
	//激活GPIOB时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);   

	//分别配置PB8、PB9推挽输出
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOB, &gpio_InitStruct);

	//初始时CLK和DATA拉低
	GPIO_ResetBits (GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
}

//往RGB写入u32数据，高位在前（MSB）
void RGBLED_write(u32 data)
{
	int i;
	//空闲时CLK拉低
	CLK_L;
	//传输32位数据
	for(i=31;i>=0;i--) //使用该形式时确保变量i是 有符号整型
	{
		//时钟线拉高，准备数据
		CLK_H;
		if(data & (1<<i))
		{
			DATA_H;
		}
		else
		{
			DATA_L;
		}
		//时钟线拉高延时
		SysTick_Delayus(5);
		
		//时钟线拉低，出现下降沿，发送一位数据
		CLK_L;
		//时钟线拉低延时
		SysTick_Delayus(5);
	}

	//空闲时CLK拉低
	CLK_L;
}

//RGB显示颜色
//参数：u32 color 采用R[23:16],G[15:8],B[7:0]格式
void RGBLED_Display(u32 color)
{
	u32 data = 0;
	u8 r,g,b;
	r = (u8)(color >> 16);
	g = (u8)(color >> 8);
	b = (u8)(color >> 0);
	//填充标志位
	data |= (0x3 << 30);
	//填充校验码
	data |= ((b ^ 0xC0)>>6) << 28;
	data |= ((g ^ 0xC0)>>6) << 26;
	data |= ((r ^ 0xC0)>>6) << 24;
	data |= (b << 16);
	data |= (g << 8);
	data |= (r << 0);
	//传输起始帧
	RGBLED_write(0);
	//传输第一个点数据
	RGBLED_write(data);
}
