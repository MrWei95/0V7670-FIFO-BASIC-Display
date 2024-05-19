#include "timer.h"

uint8_t TIM_1S = 0;
uint16_t FS_Cnt = 0;

void TIMER_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	TIM_InitStructure.TIM_Period = 10000-1;                    //自动重装载值，5000us进入一次中断
	TIM_InitStructure.TIM_Prescaler = 7200-1;                  //定时器分频
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //定时器时钟分频
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;    //计数模式
	
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);                 //定时器初始化
	TIM_Cmd(TIM2,ENABLE);         	                           //使能定时器
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                   //溢出中断
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		TIM_1S = 1;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                //清除中断标志位
	}
}
