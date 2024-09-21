#include "exti.h"
#include "ov7670.h"


void mEXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;								//定义结构体
    
	//时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);				//开启EXTI的时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;               
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;              
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;              
	GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);			//选择EXTI的信号源
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;							//中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;					//EXTI为中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;				//上升沿中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;							//使能中断
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;					//使能外部中断所在的通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			//抢占优先级0 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//使能外部中断通道 
    NVIC_Init(&NVIC_InitStructure);										//根据结构体信息进行优先级初始化
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET)								//是8线的中断
	{      
		if(OV7670_STA < 2)
		{
			if(OV7670_STA == 0)
			{
				FIFO_WRST = 0;											//复位写指针		  		 
				FIFO_WRST = 1;	
				FIFO_WEN = 1;											//允许写入FIFO 	  
			}else
			{
				FIFO_WEN = 0;
				FIFO_WRST = 0;
				FIFO_WRST = 1;
			}
			OV7670_STA++;												//帧中断加1 
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2);									//清除EXTI8线路挂起位						
}
