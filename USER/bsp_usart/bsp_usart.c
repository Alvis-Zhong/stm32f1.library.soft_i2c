/******
    ***********************************************************
    *@file	bsp_usart.c
    *@author	Bin
    *@brief	串口的配置文件
    ***********************************************************
******/


#include "bsp_usart.h"

void usart_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef usart_InitStruct;
	
	RCC_APB2PeriphClockCmd( USART_CLK , ENABLE ) ; 
	RCC_APB2PeriphClockCmd( USART_GPIO_CLK , ENABLE ) ; 
	
	/*  发送管脚设置 */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ; //推挽复用 
	GPIO_InitStruct.GPIO_Pin = USART_GPIO_PIN_TX ; 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ; 
	GPIO_Init( USART_GPIO_PORT , &GPIO_InitStruct ) ; 
	
	/*  接收管脚设置 */
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ; //浮空输入
//	GPIO_InitStruct.GPIO_Pin = USART_GPIO_PIN_RX ; 
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ; 
//	GPIO_Init( USART_GPIO_PORT , &GPIO_InitStruct ) ; 
	
	usart_InitStruct.USART_BaudRate = 115200 ; 
	usart_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ; 
	usart_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;
	usart_InitStruct.USART_Parity = USART_Parity_No ; 
	usart_InitStruct.USART_StopBits = USART_StopBits_1 ;  
	usart_InitStruct.USART_WordLength = USART_WordLength_8b ;  
	USART_Init( USART_PORT , &usart_InitStruct ) ; 
	
	USART_Cmd( USART_PORT , ENABLE ) ; 		
}

//重定向c库函数prinft到usart1
int fputc(int ch,FILE *f) 
{
		USART_SendData(USART1, (uint8_t) ch) ; //发送一个字节数据到USART1
	
		while(USART_GetFlagStatus( USART1 , USART_FLAG_TXE ) == RESET ) ; //等待发送完成
	
		return(ch) ;
}

//重定向c库函数scanf到usart1
//int fgetc(FILE *f) 
//{
//		while(USART_GetFlagStatus( USART1 , USART_FLAG_RXNE ) == RESET ) ; //等待接收完成
//	
//		return (int)USART_ReceiveData(USART1) ; 
//}
