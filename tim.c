#include <stm32f10x.h>

//TIM1毫秒级延时
//工作频率：72MHz
//  arr/(72M/psc) == xms/1000
// arr*psc == xms*72000
void TIM1_Delayms(u32 xms)
{
	//定义TIM核心结构体变量
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitStruct;
	
	//激活TIM1的时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1, ENABLE);
	
	//配置TIM1的时基单元
	tim_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1 ;
	tim_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up ;
	tim_TimeBaseInitStruct.TIM_Period = 10*xms - 1 ; //下次更新事件后重装载寄存器值
	tim_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1 ; //预分频寄存器值
	tim_TimeBaseInitStruct.TIM_RepetitionCounter = 0 ;

	TIM_TimeBaseInit (TIM1, &tim_TimeBaseInitStruct);
	
	//产生更新事件（更新ARR值）
	TIM_GenerateEvent (TIM1, TIM_EventSource_Update);
	TIM_ClearFlag (TIM1, TIM_FLAG_Update);
	
	//计数器清零
	TIM_SetCounter (TIM1, 0);
	
	//TIM1启动计数
	TIM_Cmd (TIM1, ENABLE);
	
	//等待计数完成，停止计数，清除标志位
	while(TIM_GetFlagStatus (TIM1, TIM_FLAG_Update) != SET);
	TIM_Cmd (TIM1, DISABLE);
	TIM_ClearFlag (TIM1, TIM_FLAG_Update);
}
