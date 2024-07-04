#include "sys.h"
#include "SCCB.h"
#include "delay.h"

void SCCB_Init(void)
{			
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = SCCB_SCL_PIN;				 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
 	GPIO_Init(SCCB_SCL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SCCB_SDA_PIN;				 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SCCB_SDA_PORT, &GPIO_InitStructure);
	
 	GPIO_SetBits(SCCB_SCL_PORT,SCCB_SCL_PIN);
	GPIO_SetBits(SCCB_SDA_PORT,SCCB_SDA_PIN);		
 
	SCCB_SDA_OUT();	   
}			 

void SCCB_Start(void)
{
	SCCB_SDA=1;				//数据线高电平	   
	SCCB_SCL=1;				//在时钟线高的时候数据线由高至低
	delay_us(50);  
	SCCB_SDA=0;
	delay_us(50);	 
	SCCB_SCL=0;				//数据线恢复低电平，单操作函数必要	  
}

void SCCB_Stop(void)
{
	SCCB_SDA=0;
	delay_us(50);	 
	SCCB_SCL=1;	
	delay_us(50); 
	SCCB_SDA=1;	
	delay_us(50);
}  

void SCCB_No_Ack(void)
{
	delay_us(50);
	SCCB_SDA=1;	
	SCCB_SCL=1;	
	delay_us(50);
	SCCB_SCL=0;	
	delay_us(50);
	SCCB_SDA=0;	
	delay_us(50);
}

u8 SCCB_WR_Byte(u8 dat)
{
	u8 j,res;	 
	for(j=0;j<8;j++)		//循环8次发送数据
	{
		if(dat&0x80)SCCB_SDA=1;	
		else SCCB_SDA=0;
		dat<<=1;
		delay_us(50);
		SCCB_SCL=1;	
		delay_us(50);
		SCCB_SCL=0;		   
	}			 
	SCCB_SDA_IN();			//设置SDA为输入 
	delay_us(50);
	SCCB_SCL=1;				//接收第九位,以判断是否发送成功
	delay_us(50);
	if(SCCB_READ_SDA)res=1;	//SDA=1发送失败，返回1
	else res=0;				//SDA=0发送成功，返回0
	SCCB_SCL=0;		 
	SCCB_SDA_OUT();			//设置SDA为输出    
	return res;  
}	 

u8 SCCB_RD_Byte(void)
{
	u8 temp=0,j;    
	SCCB_SDA_IN();			//设置SDA为输入  
	for(j=8;j>0;j--)		//循环8次接收数据
	{		     	  
		delay_us(50);
		SCCB_SCL=1;
		temp=temp<<1;
		if(SCCB_READ_SDA)temp++;   
		delay_us(50);
		SCCB_SCL=0;
	}	
	SCCB_SDA_OUT();			//设置SDA为输出    
	return temp;
} 							    

u8 SCCB_WR_Reg(u8 reg,u8 data)
{
	u8 res=0;
	SCCB_Start();			//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))res=1;	//写器件ID	  
	delay_us(100);
	if(SCCB_WR_Byte(reg))res=1;		//写寄存器地址	  
	delay_us(100);
	if(SCCB_WR_Byte(data))res=1;	//写数据	 
	SCCB_Stop();	  
	return	res;
}		  					    

u8 SCCB_RD_Reg(u8 reg)
{
	u8 val=0;
	SCCB_Start();			//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);	//写器件ID	  
	delay_us(100);	 
  SCCB_WR_Byte(reg);		//写寄存器地址	  
	delay_us(100);	  
	SCCB_Stop();   
	delay_us(100);	   
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(SCCB_ID|0X01);	//发送读命令	  
	delay_us(100);
	val=SCCB_RD_Byte();		//读取数据
	SCCB_No_Ack();
	SCCB_Stop();
	return val;
}
