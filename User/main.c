/*
	本人项目只在GitHub开源，
	有任何问题都可以联系我邮箱：mrwei95@outlook.com
	GitHub项目地址：https://github.com/MrWei95/0V7670-FIFO-BASIC-Display
*/

#include "stm32f10x.h"
#include "DELAY.H"
#include "OV7670.H"
#include "EXTI.H"
#include "Timer.H"
#include "ST7789.H"
void Camera_Refresh(void);


int main(void)
{
	RCC_Configuration();		//时钟设置
	
	ST7789_Init();				//屏幕初始化
	OV7670_Init();				//摄像头初始化
	mEXTI_Init();				//外部中断初始化
	TIMER_Init();				//定时器初始化
	
	while(1)
	{
		Camera_Refresh();		//读取摄像头图像
	}
}

void Camera_Refresh(void)
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
