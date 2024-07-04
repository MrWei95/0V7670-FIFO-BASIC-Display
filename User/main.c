/*
	本人项目只在GitHub开源！
	有任何问题都可以联系我邮箱：mrwei95@outlook.com
	GitHub项目地址：https://github.com/MrWei95/0V7670-FIFO-BASIC-Display
*/

#include "stm32f10x.h"                  // Device header
#include "DELAY.H"
#include "OV7670.H"
#include "ST7789.H"
#include "image.h"
#include "exti.h"
#include "timer.h"
void Camera_Refresh(void);


int main(void)
{
	RCC_Configuration();								// 时钟设置
	
	ST7789_Init();										// 屏幕初始化
	OV7670_Init();										// 摄像头初始化
	mEXTI_Init();										// 外部中断初始化
	TIMER_Init();										// 定时器初始化
	
	LCD_Fill(0, 0, LCD_WIDTH, LCD_HIGH, BLACK);			//屏幕涂黑
	LCD_DrawBMP(0, 0, 198, 160, Image_95);				//显示图像
	delay_s(3);
	LCD_Fill(0, 0, LCD_WIDTH, LCD_HIGH, BLACK);			//屏幕再次涂黑
	
	while(1)
	{
		Camera_Refresh();								// 读取摄像头图像
	}
}


//摄像头主程序
void Camera_Refresh(void)
{
	uint32_t i, j;
	uint16_t color;
	uint16_t colorL, colorH;
	uint16_t imageBuffer[320];							// 用于存储一行的像素数据
	
	if(OV7670_STA == 2)									// 有帧中断更新
	{
		ST7789_A0 = 1;
		LCD_SetWindows(0, 0, LCD_WIDTH-1, LCD_HIGH-1);
		FIFO_RRST = 0;									// 开始复位读指针
		FIFO_RCLK = 0;
		FIFO_RCLK = 1;
		FIFO_RCLK = 0;
		FIFO_RRST = 1;									// 复位读指针结束
		FIFO_RCLK = 1;
		for(i = 0; i < LCD_HIGH; i++)					// 遍历每一行
		{
			// 读取一行的数据
			for(j = 0; j < LCD_WIDTH; j++)
			{
				FIFO_RCLK = 0;
				colorH = OV7670_RedData();				// 读高8位数据
				FIFO_RCLK = 1;
				FIFO_RCLK = 0;
				colorL = OV7670_RedData();				// 读低8位数据
				colorL >>= 8;
				color = colorH | colorL;
				FIFO_RCLK = 1;

				imageBuffer[j] = color;					// 存储在缓冲区中
			}

			// 将缓冲区中的数据倒序写入屏幕
			for(j = 0; j < LCD_WIDTH; j++)
			{
				LCD_WR_DATA_16Bit(imageBuffer[LCD_WIDTH - 1 - j]);
			}
		}
		OV7670_STA = 0;									// 清零帧中断标记
	}
}
