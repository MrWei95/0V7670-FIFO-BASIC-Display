///////////////////////////////////////////////////////////////////////////////
//大家如果代码有任何问题请联系我：mrwei95@outlook.com
//GitHub项目地址：https://github.com/MrWei95/0V7670-FIFO-BASIC-Display
///////////////////////////////////////////////////////////////////////////////

#include "stm32f10x.h"
#include "delay.h"
#include "uart.h"
#include "ov7670.h"
#include "sccb.h"
#include "exti.h"
#include "timer.h"
#include "st7789.h"
void camera_refresh(void);


int main(void)
{
	RCC_Configuration();		//时钟设置
	
	USART1_Init(115200);		//串口初始化
	ST7789_Init();				//屏幕初始化
	OV7670_Init();				//摄像头初始化
	mEXTI_Init();				//外部中断初始化
	TIMER_Init();				//定时器初始化
	
	while(1)
	{
		camera_refresh();		//读取摄像头图像
	}
}


void camera_refresh(void)
{
	uint32_t j;
 	uint16_t color;	
	uint16_t colorL,colorH;
	if(OV7670_STA == 2)								//有帧中断更新
	{ 
		ST7789_A0 = 1;  
		LCD_SetWindows(0,0,LCD_WIDTH-1,LCD_HIGH-1);
		FIFO_RRST = 0;								//开始复位读指针 
		FIFO_RCLK = 0;
		FIFO_RCLK = 1;
		FIFO_RCLK = 0;
		FIFO_RRST = 1;								//复位读指针结束 
		FIFO_RCLK = 1;
		for(j = 0; j < 76800; j++)					//76800  
		{
			FIFO_RCLK = 0;
			colorH = OV7670_RedData();				//读数据
			FIFO_RCLK = 1;
			FIFO_RCLK = 0;
			colorL = OV7670_RedData();				//读数据
			colorL >>= 8; 
			color = colorH | colorL;
			FIFO_RCLK = 1; 
			LCD_WR_DATA_16Bit(color);				//屏幕显示
		}   							  
 		OV7670_STA = 0;								//清零帧中断标记
	}
}
