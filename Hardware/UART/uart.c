#include "uart.h"
#include "stdio.h"

uint8_t uart_dat = 0;

//重定义fputc函数 
int fputc(int ch,FILE *f)
{      
	while((USART1->SR & 0X40)==0){};   
  USART1->DR = (u8) ch;      
	return ch;
}

void USART1_Init(int Buad)
{
	GPIO_InitTypeDef            GPIO_InitStrue;
	USART_InitTypeDef           USART_InitStrue;
	NVIC_InitTypeDef            NVIC_InitStrue;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);     //打开GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);    //打开串口时钟
	
	GPIO_InitStrue.GPIO_Pin = GPIO_Pin_9;                    //TX
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;                //复用推挽
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;              //设置最大输出速度
	GPIO_Init(GPIOA,&GPIO_InitStrue);                        //GPIO端口初始化设置
	
	GPIO_InitStrue.GPIO_Pin = GPIO_Pin_10;                   //RX
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IPU;                  //上拉输入
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;              //设置最大输出速度
	GPIO_Init(GPIOA,&GPIO_InitStrue);                        //GPIO端口初始化设置
	
	USART_InitStrue.USART_BaudRate = Buad;                   //波特率
	USART_InitStrue.USART_WordLength = USART_WordLength_8b;  //数据位8位
	USART_InitStrue.USART_StopBits = USART_StopBits_1;       //停止位1位
	USART_InitStrue.USART_Parity = USART_Parity_No;          //无校验位
	USART_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStrue.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStrue);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStrue.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStrue);
	
	USART_Cmd(USART1,ENABLE);
}

void USART_SendByte(USART_TypeDef *USARTx,char str)
{
	USART_SendData(USARTx,str);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void USART_SendString(USART_TypeDef *USARTx,char *str)
{
	while(*str != '\0')
	{
		USART_SendData(USARTx,*str);
		str++;
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	}
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		uart_dat = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
