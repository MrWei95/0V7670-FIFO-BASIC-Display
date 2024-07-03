#include "OV7670.H"
#include "DELAY.H"
#include "SCCB.H"

uint8_t  OV7670_STA = 0;

const u8 ov7670_init_reg[][2] = 
{   
/*以下为OV7670 QVGA RGB565参数  */
	{0x3a, 0x04},//dummy
	{0x40, 0xd0},//565   
	{0x12, 0x14},//QVGA,RGB输出

	//输出窗口设置
	{0x32, 0x80},//HREF control	bit[2:0] HREF start 3 LSB	 bit[5:3] HSTOP HREF end 3LSB
	{0x17, 0x16},//HSTART start high 8-bit MSB  0x16   0x17    
	{0x18, 0x04},//5 HSTOP end high 8-bit       0x04   0x05
	{0x19, 0x02},
	{0x1a, 0x7b},//0X7B,0x7a,
	{0x03, 0x06},//0X06,0x0a,帧竖直方向控制

	{0x0c, 0x00},
	{0x15, 0x00},//0x00
	{0x3e, 0x00},//10
	{0x70, 0x3a},
	{0x71, 0x35},
	{0x72, 0x11},
	{0x73, 0x00},//

	{0xa2, 0x02},//15
	{0x11, 0x81},//时钟分频设置,0不分频   设置1=2分频
	{0x7a, 0x20},
	{0x7b, 0x1c},
	{0x7c, 0x28},

	{0x7d, 0x3c},//20
	{0x7e, 0x55},
	{0x7f, 0x68},
	{0x80, 0x76},
	{0x81, 0x80},

	{0x82, 0x88},
	{0x83, 0x8f},
	{0x84, 0x96},
	{0x85, 0xa3},
	{0x86, 0xaf},

	{0x87, 0xc4},//30
	{0x88, 0xd7},
	{0x89, 0xe8},
	{0x13, 0xe0},
	{0x00, 0x00},//AGC

	{0x10, 0x00},
	{0x0d, 0x00},//全窗口， 位[5:4]: 01 半窗口，10 1/4窗口，11 1/4窗口 
	{0x14, 0x28},//0x38, limit the max gain
	{0xa5, 0x05},
	{0xab, 0x07},

	{0x24, 0x75},//40
	{0x25, 0x63},
	{0x26, 0xA5},
	{0x9f, 0x78},
	{0xa0, 0x68},

	{0xa1, 0x03},//0x0b,
	{0xa6, 0xdf},//0xd8,
	{0xa7, 0xdf},//0xd8,
	{0xa8, 0xf0},
	{0xa9, 0x90},

	{0xaa, 0x94},//50
	{0x13, 0xe5},
	{0x0e, 0x61},
	{0x0f, 0x4b},
	{0x16, 0x02},

	{0x1e, 0x27},//图像输出镜像控制.0x07
	{0x21, 0x02},
	{0x22, 0x91},
	{0x29, 0x07},
	{0x33, 0x0b},

	{0x35, 0x0b},//60
	{0x37, 0x1d},
	{0x38, 0x71},
	{0x39, 0x2a},
	{0x3c, 0x78},

	{0x4d, 0x40},
	{0x4e, 0x20},
	{0x69, 0x00},
	{0x6b, 0x40},//PLL*4=48Mhz
	{0x74, 0x19},
	{0x8d, 0x4f},

	{0x8e, 0x00},//70
	{0x8f, 0x00},
	{0x90, 0x00},
	{0x91, 0x00},
	{0x92, 0x00},//0x19,//0x66

	{0x96, 0x00},
	{0x9a, 0x80},
	{0xb0, 0x84},
	{0xb1, 0x0c},
	{0xb2, 0x0e},

	{0xb3, 0x82},//80
	{0xb8, 0x0a},
	{0x43, 0x14},
	{0x44, 0xf0},
	{0x45, 0x34},

	{0x46, 0x58},
	{0x47, 0x28},
	{0x48, 0x3a},
	{0x59, 0x88},
	{0x5a, 0x88},

	{0x5b, 0x44},//90
	{0x5c, 0x67},
	{0x5d, 0x49},
	{0x5e, 0x0e},
	{0x64, 0x04},
	{0x65, 0x20},

	{0x66, 0x05},
	{0x94, 0x04},
	{0x95, 0x08},
	{0x6c, 0x0a},
	{0x6d, 0x55},


	{0x4f, 0x80},
	{0x50, 0x80},
	{0x51, 0x00},
	{0x52, 0x22},
	{0x53, 0x5e},
	{0x54, 0x80},

	//{0x54, 0x40},//110


	{0x09, 0x03},//驱动能力最大

	{0x6e, 0x11},//100
	{0x6f, 0x9f},//0x9e for advance AWB
	{0x55, 0x00},//亮度
	{0x56, 0x40},//对比度 0x40
	{0x57, 0x40},//0x40,  change according to Jim's request

//以下部分代码由开源电子网网友:duanzhang512 提出
//添加此部分代码将可以获得更好的成像效果,但是最下面一行会有蓝色的抖动.
//如不想要,可以屏蔽此部分代码.然后将:OV7670_Window_Set(12,176,240,320);
//改为:OV7670_Window_Set(12,174,240,320);,即可去掉最下一行的蓝色抖动
	{0x6a, 0x40},
	{0x01, 0x40},
	{0x02, 0x40},
	{0x13, 0xe7},
	{0x15, 0x00},  
	
		
	{0x58, 0x9e},
	
	{0x41, 0x08},
	{0x3f, 0x00},
	{0x75, 0x05},
	{0x76, 0xe1},
	{0x4c, 0x00},
	{0x77, 0x01},
	{0x3d, 0xc2},	
	{0x4b, 0x09},
	{0xc9, 0x60},
	{0x41, 0x38},
	
	{0x34, 0x11},
	{0x3b, 0x02}, 

	{0xa4, 0x89},
	{0x96, 0x00},
	{0x97, 0x30},
	{0x98, 0x20},
	{0x99, 0x30},
	{0x9a, 0x84},
	{0x9b, 0x29},
	{0x9c, 0x03},
	{0x9d, 0x4c},
	{0x9e, 0x3f},
	{0x78, 0x04},
	
	{0x79, 0x01},
	{0xc8, 0xf0},
	{0x79, 0x0f},
	{0xc8, 0x00},
	{0x79, 0x10},
	{0xc8, 0x7e},
	{0x79, 0x0a},
	{0xc8, 0x80},
	{0x79, 0x0b},
	{0xc8, 0x01},
	{0x79, 0x0c},
	{0xc8, 0x0f},
	{0x79, 0x0d},
	{0xc8, 0x20},
	{0x79, 0x09},
	{0xc8, 0x80},
	{0x79, 0x02},
	{0xc8, 0xc0},
	{0x79, 0x03},
	{0xc8, 0x40},
	{0x79, 0x05},
	{0xc8, 0x30},
	{0x79, 0x26}, 
	{0x09, 0x00},
	
};


void OV7670_XCLK_ON(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    RCC_MCOConfig(RCC_MCO_HSE);
}


void OV7670_XCLK_OFF(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//OV7670功能设置
//白平衡设置
//0:自动
//1:太阳sunny
//2,阴天cloudy
//3,办公室office
//4,家里home
void OV7670_Light_Mode(u8 mode)
{
	u8 reg13val=0XE7;			//默认就是设置为自动白平衡
	u8 reg01val=0;
	u8 reg02val=0;
	switch(mode)
	{
		case 1:					//sunny
			reg13val=0XE5;
			reg01val=0X5A;
			reg02val=0X5C;
			break;
		case 2:					//cloudy
			reg13val=0XE5;
			reg01val=0X58;
			reg02val=0X60;
			break;
		case 3:					//office
			reg13val=0XE5;
			reg01val=0X84;
			reg02val=0X4c;
			break;
		case 4:					//home
			reg13val=0XE5;
			reg01val=0X96;
			reg02val=0X40;
			break;
	}
	SCCB_WR_Reg(0X13,reg13val);	//COM8设置
	SCCB_WR_Reg(0X01,reg01val);	//AWB蓝色通道增益
	SCCB_WR_Reg(0X02,reg02val);	//AWB红色通道增益
}

//色度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Color_Saturation(u8 sat)
{
	u8 reg4f5054val=0X80;//默认就是sat=2,即不调节色度的设置
 	u8 reg52val=0X22;
	u8 reg53val=0X5E;
 	switch(sat)
	{
		case 0:						//-2
			reg4f5054val=0X40;
			reg52val=0X11;
			reg53val=0X2F;
			break;
		case 1:						//-1
			reg4f5054val=0X66;
			reg52val=0X1B;
			reg53val=0X4B;
			break;
		case 3:						//1
			reg4f5054val=0X99;
			reg52val=0X28;
			reg53val=0X71;
			break;
		case 4:						//2
			reg4f5054val=0XC0;
			reg52val=0X33;
			reg53val=0X8D;
			break;
	}
	SCCB_WR_Reg(0X4F,reg4f5054val);	//色彩矩阵系数1
	SCCB_WR_Reg(0X50,reg4f5054val);	//色彩矩阵系数2 
	SCCB_WR_Reg(0X51,0X00);			//色彩矩阵系数3  
	SCCB_WR_Reg(0X52,reg52val);		//色彩矩阵系数4 
	SCCB_WR_Reg(0X53,reg53val);		//色彩矩阵系数5 
	SCCB_WR_Reg(0X54,reg4f5054val);	//色彩矩阵系数6  
	SCCB_WR_Reg(0X58,0X9E);			//MTXS 
}

//亮度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Brightness(u8 bright)
{
	u8 reg55val=0X00;//默认就是bright=2
  	switch(bright)
	{
		case 0:					//-2
			reg55val=0XB0;
			break;
		case 1:					//-1
			reg55val=0X98;
			break;
		case 3:					//1
			reg55val=0X18;
			break;
		case 4:					//2
			reg55val=0X30;
			break;
	}
	SCCB_WR_Reg(0X55,reg55val);	//亮度调节 
}

//对比度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Contrast(u8 contras)
{
	u8 reg56val=0X40;			//默认就是contrast=2
	switch(contras)
	{
		case 0:					//-2
			reg56val=0X30;	 	 
			break;
		case 1:					//-1
			reg56val=0X38;
			break;
		case 3:					//1
			reg56val=0X50;
			break;	
		case 4:					//2
			reg56val=0X60;
			break;
	}
	SCCB_WR_Reg(0X56,reg56val);	//对比度调节
}

//特效设置
//0:普通模式    
//1,负片
//2,黑白   
//3,偏红色
//4,偏绿色
//5,偏蓝色
//6,复古	    
void OV7670_Special_Effects(u8 eft)
{
	u8 reg3aval=0X04;			//默认为普通模式
	u8 reg67val=0XC0;
	u8 reg68val=0X80;
	switch(eft)
	{
		case 1:					//负片
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;
		case 2:					//黑白
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;
		case 3:					//偏红色
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;
		case 4:					//偏绿色
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;
		case 5:					//偏蓝色
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;
		case 6:					//复古
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;
	}
	SCCB_WR_Reg(0X3A,reg3aval);	//TSLB设置
	SCCB_WR_Reg(0X68,reg67val);	//MANU,手动U值
	SCCB_WR_Reg(0X67,reg68val);	//MANV,手动V值
}

//设置图像输出窗口
//对QVGA设置。
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp; 
	endx=sx+width*2;					//V*2
 	endy=sy+height*2;
	if(endy>784)endy-=784;
	temp=SCCB_RD_Reg(0X03);				//读取Vref之前的值
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	SCCB_WR_Reg(0X03,temp);				//设置Vref的start和end的最低2位
	SCCB_WR_Reg(0X19,sx>>2);			//设置Vref的start高8位
	SCCB_WR_Reg(0X1A,endx>>2);			//设置Vref的end的高8位

	temp=SCCB_RD_Reg(0X32);				//读取Href之前的值
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(sy&0X07);
	SCCB_WR_Reg(0X17,sy>>3);			//设置Href的start高8位
	SCCB_WR_Reg(0X18,endy>>3);			//设置Href的end的高8位
}


void OV7670_SetWindow(u16 startx,u16 starty,u16 width,u16 height)
{
	uint16_t endx;
	uint16_t endy;
	uint8_t x_reg;
	uint8_t y_reg;
	uint8_t temp;
	
	endx = (startx + width*2)%784;
	endy = (starty + height*2);
	
	x_reg = SCCB_RD_Reg(0x32);
	x_reg &= 0xc0;
	y_reg = SCCB_RD_Reg(0x03);
	y_reg &= 0xf0;
	
	//设置HREF
	temp = x_reg | ((endx & 0x7) << 3) | (startx & 0x7);
	SCCB_WR_Reg(0x32,temp);
	temp = (startx & 0x7f8) >> 3;
	SCCB_WR_Reg(0x17,temp);
	temp = (endx & 0x7f8) >> 3;
	SCCB_WR_Reg(0x18,temp);
	
	//设置VREF
	temp = y_reg | ((endy & 0x3) << 2) | (starty & 0x3);
	SCCB_WR_Reg(0x03,temp);
	temp = (starty & 0x3fc) >> 2;
	SCCB_WR_Reg(0x19,temp);
	temp = (endy & 0x3fc) >> 2;
	SCCB_WR_Reg(0x1a,temp);
}

unsigned char OV7670_Init(void)
{
	unsigned char i;
	GPIO_InitTypeDef GPIO_InitStructure;
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
		| RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);	 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//禁止JTAG,从而PA15可以做普通IO使用,否则PA15不能做普通IO!!!
	
	GPIO_InitStructure.GPIO_Pin =  OV7670_VSYNC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(OV7670_VSYNC_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FIFO_WRST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FIFO_WRST_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FIFO_WEN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FIFO_WEN_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FIFO_RRST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FIFO_RRST_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FIFO_OE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FIFO_OE_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FIFO_RCLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FIFO_RCLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10
		| GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OV7670_XCLK_ON();
	
	FIFO_OE = 0;
	
	SCCB_Init();        		//初始化SCCB 的IO口	   	  
// 	printf("temp1:%d",SCCB_WR_Reg(0x12,0x80));
//	delay_ms(50);
//	//读取产品型号
// 	printf("temp2:%d",SCCB_RD_Reg(0x0a));
// 	printf("temp3:%d",SCCB_RD_Reg(0x0b));
//
//	LCD_ShowNum(0,0,WHITE,BLACK,SCCB_WR_Reg(0x12,0x80),16);
//	LCD_ShowNum(0,50,WHITE,BLACK,SCCB_RD_Reg(0x0a),16);
//	LCD_ShowNum(0,100,WHITE,BLACK,SCCB_RD_Reg(0x0b),16);

	for(i = 0; i < sizeof(ov7670_init_reg)/sizeof(ov7670_init_reg[0]); i++)
	{
		SCCB_WR_Reg(ov7670_init_reg[i][0],ov7670_init_reg[i][1]);
	}
	
	OV7670_Light_Mode(lightmode);			//白平衡设置0~4; 0
	OV7670_Color_Saturation(saturation);	//色度设置0~4; 2
	OV7670_Brightness(brightness);			//亮度设置0~4; 2
	OV7670_Contrast(contrast);				//对比度设置0~4; 2
 	OV7670_Special_Effects(effect);			//特效设置0~6; 0
	//OV7670_Window_Set(12,176,240,320);	//设置窗口
	OV7670_Window_Set(12,176,240,320);		//设置窗口
	//OV7670_SetWindow(184,10,128,128);
	
	return 0;
}
