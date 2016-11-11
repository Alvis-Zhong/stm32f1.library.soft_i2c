#ifndef __EE_H
#define __EE_H

#include "stm32f10x.h"
#include "bsp_i2c_gpio.h"
#include "bsp_usart.h"

/*	EEPRAM配置相关的宏定义	*/
#define		EE_ADD		0xA0	//EEPRAM设备地址
#define		EE_PAGE		8	//EEPRAM的页面大小
#define		EE_SIZE		256	//EEPRAM的总容量
/* 	函数声明	*/
uint8_t EE_check(void) ; 
uint8_t EE_readbits( uint8_t *data , uint16_t ee_data_add ,uint16_t ee_read_size ) ; 
uint8_t EE_writebits( uint8_t *wr_data , uint16_t wr_add , uint16_t wr_size ) ; 
void EE_erase(void) ; 
void EE_test(void) ;
#endif 


