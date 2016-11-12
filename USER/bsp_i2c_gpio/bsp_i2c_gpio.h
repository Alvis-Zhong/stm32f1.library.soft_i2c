#ifndef __BSP_I2C_GPIO_H
#define __BSP_I2C_GPIO_H

#include "stm32f10x.h"
#include <inttypes.h>

/*	I2C管脚宏定义		*/
#define 	I2C_GPIO_CLK		RCC_APB2Periph_GPIOB
#define 	I2C_GPIO_PORT		GPIOB
#define 	I2C_GPIO_PIN_SCL	GPIO_Pin_6
#define 	I2C_GPIO_PIN_SDA	GPIO_Pin_7

/*	I2C读写信号宏定义	*/
#define 	I2C_WR		0
#define		I2C_RD		1

/*	I2C的SDA,SCL控制信号宏定义	*/
#define 	I2C_SCL_0		GPIO_ResetBits( I2C_GPIO_PORT , I2C_GPIO_PIN_SCL )
#define 	I2C_SCL_1		GPIO_SetBits( I2C_GPIO_PORT , I2C_GPIO_PIN_SCL )
#define 	I2C_SDA_0		GPIO_ResetBits( I2C_GPIO_PORT , I2C_GPIO_PIN_SDA )
#define 	I2C_SDA_1		GPIO_SetBits( I2C_GPIO_PORT , I2C_GPIO_PIN_SDA )
#define		I2C_READ_DATA		GPIO_ReadInputDataBit( I2C_GPIO_PORT , I2C_GPIO_PIN_SDA)

/*	函数声明	*/
void i2c_start(void) ;		//起始信号函数声明
void i2c_end(void) ;		//结束信号函数声明
void i2c_writebits(uint8_t data) ; 	//CPU发送数据函数声明
uint8_t i2c_readbits(void) ; 	//CPU读取数据函数声明
void i2c_ack(void) ; 		//CPU发送应答信号函数声明
void i2c_uack(void) ; 		//CPU发送非应答信号声明
uint8_t i2c_waitack(void) ; 	//CPU等待I2C总线设备应答信号函数声明
uint8_t i2c_check_device(uint8_t device_address ) ; //CPU发送设备地址检测I2C总线设备函数声明

#endif

