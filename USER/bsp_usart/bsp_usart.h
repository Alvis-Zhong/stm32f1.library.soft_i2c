#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stdio.h"
/*	串口相关配置宏定义	*/
#define		USART_PORT		USART1
#define 	USART_CLK		RCC_APB2Periph_USART1
#define		USART_GPIO_PORT		GPIOA
#define 	USART_GPIO_CLK		RCC_APB2Periph_GPIOA
#define 	USART_GPIO_PIN_TX	GPIO_Pin_9
#define 	USART_GPIO_PIN_RX	GPIO_Pin_10

void usart_config(void);

#endif 
