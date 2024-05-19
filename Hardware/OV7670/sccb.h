#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"
  
#define SCCB_SCL_PORT    		GPIOA	 	
#define SCCB_SDA_PORT    		GPIOA		
#define SCCB_SCL_PIN    		GPIO_Pin_0	 //PA0连接SCL 	
#define SCCB_SDA_PIN    		GPIO_Pin_1   //PA1连接SDA

#define SCCB_SDA_IN()       {GPIOA->CRL &= 0XFFFFFF0F;GPIOA->CRL |= 0X00000080;}
#define SCCB_SDA_OUT()      {GPIOA->CRL &= 0XFFFFFF0F;GPIOA->CRL |= 0X00000030;}
	 
#define SCCB_SCL    		    PAout(0)	 	
#define SCCB_SDA    		    PAout(1) 		

#define SCCB_READ_SDA       PAin(1)  		  
#define SCCB_ID   			    0X42  			

void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);

#endif
