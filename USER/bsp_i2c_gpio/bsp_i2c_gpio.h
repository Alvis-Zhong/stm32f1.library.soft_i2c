#ifndef __BSP_I2C_GPIO_H
#define __BSP_I2C_GPIO_H

#include "stm32f10x.h"
#include <inttypes.h>

/*	I2C�ܽź궨��		*/
#define 	I2C_GPIO_CLK		RCC_APB2Periph_GPIOB
#define 	I2C_GPIO_PORT		GPIOB
#define 	I2C_GPIO_PIN_SCL	GPIO_Pin_6
#define 	I2C_GPIO_PIN_SDA	GPIO_Pin_7

/*	I2C��д�źź궨��	*/
#define 	I2C_WR		0
#define		I2C_RD		1

/*	I2C��SDA,SCL�����źź궨��	*/
#define 	I2C_SCL_0		GPIO_ResetBits( I2C_GPIO_PORT , I2C_GPIO_PIN_SCL )
#define 	I2C_SCL_1		GPIO_SetBits( I2C_GPIO_PORT , I2C_GPIO_PIN_SCL )
#define 	I2C_SDA_0		GPIO_ResetBits( I2C_GPIO_PORT , I2C_GPIO_PIN_SDA )
#define 	I2C_SDA_1		GPIO_SetBits( I2C_GPIO_PORT , I2C_GPIO_PIN_SDA )
#define		I2C_READ_DATA		GPIO_ReadInputDataBit( I2C_GPIO_PORT , I2C_GPIO_PIN_SDA)

/*	��������	*/
void i2c_start(void) ;		//��ʼ�źź�������
void i2c_end(void) ;		//�����źź�������
void i2c_writebits(uint8_t data) ; 	//CPU�������ݺ�������
uint8_t i2c_readbits(void) ; 	//CPU��ȡ���ݺ�������
void i2c_ack(void) ; 		//CPU����Ӧ���źź�������
void i2c_uack(void) ; 		//CPU���ͷ�Ӧ���ź�����
uint8_t i2c_waitack(void) ; 	//CPU�ȴ�I2C�����豸Ӧ���źź�������
uint8_t i2c_check_device(uint8_t device_address ) ; //CPU�����豸��ַ���I2C�����豸��������

#endif

