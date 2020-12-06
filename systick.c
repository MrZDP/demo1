#include <stm32f10x.h>

//滴答定时器毫秒级延时，单次最大定时1.86s
void SysTick_Delayms(u32 xms)
{
	//选择时钟源（选择计数频率）:9MHz
	SysTick->CTRL &=~(1<<2);
	//设置计数个数
	SysTick->LOAD = 9000*xms;
	//打开计数器
	SysTick->CTRL |= (1<<0);
	//等待计数完成
	while( !(SysTick->CTRL & (1<<16)) );
	//关闭计数器
	SysTick->CTRL &=~(1<<0);
}

//滴答定时器微秒级延时，单次最大定时1.86s
void SysTick_Delayus(u32 xus)
{
	//选择时钟源（选择计数频率）:9MHz
	SysTick->CTRL &=~(1<<2);
	//设置计数个数
	SysTick->LOAD = 9*xus;
	//打开计数器
	SysTick->CTRL |= (1<<0);
	//等待计数完成
	while( !(SysTick->CTRL & (1<<16)) );
	//关闭计数器
	SysTick->CTRL &=~(1<<0);
}