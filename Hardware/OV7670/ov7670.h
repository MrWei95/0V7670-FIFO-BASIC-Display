#ifndef _OV7660_H
#define _OV7660_H
#include "stm32f10x.h"

#define OV7670_VSYNC_PORT		GPIOA  
#define FIFO_WRST_PORT			GPIOA  
#define FIFO_WEN_PORT			GPIOA  
#define FIFO_RRST_PORT			GPIOA  
#define FIFO_OE_PORT			GPIOA  
#define FIFO_RCLK_PORT			GPIOB  

#define OV7670_VSYNC_PIN		GPIO_Pin_2   
#define FIFO_WRST_PIN			GPIO_Pin_3   
#define FIFO_WEN_PIN			GPIO_Pin_4   
#define FIFO_RRST_PIN			GPIO_Pin_11   
#define FIFO_OE_PIN				GPIO_Pin_12  
#define FIFO_RCLK_PIN			GPIO_Pin_5
												//PA0连接SCL，PA1连接SDA，在sccb.h查看
#define OV7670_VSYNC			PAin(2)                //PA2帧信号
#define FIFO_WRST				PAout(3)		           //PA3写指针复位
#define FIFO_WEN				PAout(4)		           //PA4写入FIFO使能
#define FIFO_RRST				PAout(11)  		         //PA11读指针复位
#define FIFO_OE					PAout(12)  		         //PA12片选信号(OE)
#define FIFO_RCLK				PBout(5)               //PB5读数据时钟

#define OV7670_RedData()		(GPIOB->IDR & 0XFF00)  //读数据PB8-PB15

#define lightmode	0
#define saturation	3
#define brightness	2
#define contrast	4
#define effect		0

extern uint8_t OV7670_STA;

unsigned char OV7670_Init(void);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);

#endif
