#include "exti.h"
#include "ov7670.h"


void mEXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;                         //����ṹ��
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);        //ʱ��ʹ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);         //����EXTI��ʱ��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;               
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;              
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;              
		GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);  //ѡ��EXTI���ź�Դ
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;                   //�ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;          //EXTIΪ�ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;       //�������ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                    //ʹ���ж�
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;             //ʹ���ⲿ�ж����ڵ�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    //��ռ���ȼ�0 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //�����ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //ʹ���ⲿ�ж�ͨ�� 
    NVIC_Init(&NVIC_InitStructure);                              //���ݽṹ����Ϣ�������ȼ���ʼ��
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET)	//��8�ߵ��ж�
	{      
		if(OV7670_STA < 2)
		{
			if(OV7670_STA == 0)
			{
				FIFO_WRST = 0;	    //��λдָ��		  		 
				FIFO_WRST = 1;	
				FIFO_WEN = 1;	    //����д��FIFO 	  
			}else
			{
				FIFO_WEN = 0;
				FIFO_WRST = 0;
				FIFO_WRST = 1;
			}
			OV7670_STA++;		//֡�жϼ�1 
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2);  //���EXTI8��·����λ						
}
