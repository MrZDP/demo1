#include <stm32f10x.h>

#define CNTMAX	1000
//直流电机初始化函数
//IA--PB5--TIM3_CH2
//IB--PB4--TIM3_CH1
//先规定PWM周期 10ms
void DC_Motor_init(void)
{
	/*1. 将PB4\PB5复用为TIM3_CH1\CH2*/
	
	//定义GPIO核心结构体变量
	GPIO_InitTypeDef gpio_InitStruct;
	//定义TIM核心结构体变量
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitStruct;
	//定义TIMOC核心结构体变量
	TIM_OCInitTypeDef  tim_OCInitStruct;   

	
	//激活GPIOB、AFIO时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	
	//取消SWJ的JTRST的映射
	GPIO_PinRemapConfig (GPIO_Remap_SWJ_NoJTRST, ENABLE);
	//实现TIM3的部分映射（保证PB4和PB5可以复用为TIM3的通道引脚）
	GPIO_PinRemapConfig (GPIO_PartialRemap_TIM3, ENABLE);
	
	//将PB4和PB5复用TIM3的CH2和CH1：推挽复用输出
	gpio_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	gpio_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
	gpio_InitStruct.GPIO_Speed = GPIO_Speed_10MHz ;
	GPIO_Init (GPIOB, &gpio_InitStruct);
	
	/*2. TIM3时基单元配置*/
	//激活TIM3的时钟
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);
	
	//配置TIM3的时基单元
	tim_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1 ;
	tim_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up ;
	tim_TimeBaseInitStruct.TIM_Period = CNTMAX - 1; //下次更新事件后重装载寄存器值
	tim_TimeBaseInitStruct.TIM_Prescaler = 720 - 1 ; //预分频寄存器值
	tim_TimeBaseInitStruct.TIM_RepetitionCounter = 0 ;

	TIM_TimeBaseInit (TIM3, &tim_TimeBaseInitStruct);
	
	/*3. TIM3OC1和OC2配置*/
	tim_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1 ;
	tim_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High ;
	tim_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable ;
	tim_OCInitStruct.TIM_Pulse = 0 ;
	
	TIM_OC1Init (TIM3, &tim_OCInitStruct);
	TIM_OC2Init (TIM3, &tim_OCInitStruct);
	
	
	/*4. 启动定时器 */
	TIM_Cmd (TIM3, ENABLE);
}
	

//直流电机转动函数
//参数：int v_percent 表示速度的百分比，范围-100~+100
//占空比 = CCR / ARR =  |v_percent| / 100
void DC_Motor_Run(int v_percent)
{
		if(v_percent >= 0)
		{
			//正转
			TIM_SetCompare2 (TIM3, v_percent * CNTMAX / 100);
			TIM_SetCompare1 (TIM3, 0);
		}
		else
		{
			//反转
			TIM_SetCompare2 (TIM3, 0);
			TIM_SetCompare1 (TIM3, v_percent * CNTMAX / (-100));
		}
}



