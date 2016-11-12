/******
    *************************************************************************************
    *@file	bsp_i2c_ee.c
    *@author	Bin
    *@brief	EEPROM驱动文件
    *************************************************************************************
******/

#include "bsp_i2c_ee.h"

/************************************************************
    *函数名：	EE_check()
    *形参名	无
    *函数功能	检测EEPRAM是否存在
    *返回值	无
*************************************************************/
uint8_t EE_check(void)
{
	if(i2c_check_device( EE_ADD ) == 0) 	//发送EEPRAM的地址，检测是否存在该设备
	{
		return 1 ; 			//如果存在，返回1
	}
	else
	{			
		i2c_end() ;			//并且发送I2C总线结束信号 
		return 0 ; 			//如果不存在，返回0
	}
	
}

/***************************************************************
    *函数名：	EE_readbits()
    *形参名	data:存放读取到的数据的缓冲区指针
		ee_data_add:开始读取的数据的地址
		ee_read_size:读取数据的大小
    *函数功能	CPU通过串口指定读取EEPRAM中的数据
    *返回值	返回值为0，表示读取失败；返回值为1，表示读取成功
****************************************************************/
uint8_t EE_readbits( uint8_t *data , uint16_t ee_data_add ,uint16_t ee_read_size )
{
	uint16_t i ; 				//因为AT24c02的存储大小是256个字节，所以16位足够表示
	i2c_start() ; 				//起始信号
	i2c_writebits(EE_ADD | I2C_WR) ;	//发送设备地址和读取信号
	if(i2c_waitack()!=0)			//检测是否发送成功
	{
		goto fail ; 			//未收到应答信号，读取失败
	}
	i2c_writebits((uint8_t)ee_data_add) ;	//发送读取的起始地址
	if(i2c_waitack()!=0)			//检测是否发送成功
	{
		goto fail ; 			//I2C总线设备未返回应答信号，读取失败
	}
	i2c_start() ; 				//收到应答信号，继续发送起始信号
	i2c_writebits(EE_ADD | I2C_RD) ;
	if(i2c_waitack()!=0)			//检测是否发送成功
	{
		goto fail ; 			//I2C总线设备未返回应答信号，读取失败
	}
	for( i=0 ; i<ee_read_size ; i++ )	//开始读取数据
	{
		data[i] = i2c_readbits() ;	//将读取到的数据存放到数据缓存区
		if(i!=(ee_read_size-1))
		{
			i2c_ack() ; 		//未读取完数据，每次读取完一个字节的数据发送一次应答信号
		}
		else
		{
			i2c_uack() ; 		//所有数据读取完毕后，发送非应答信号
		}
	}
	i2c_end() ; 				//发送结束信号
	return 1 ; 				//返回值为1，说明读取成功
				
	fail: 	i2c_end() ; 			//读取失败，发送结束信号
		return 0 ; 			//返回值为0，说明读取失败
}

/***************************************************************
    *函数名：	EE_writebits()
    *形参名	wr_data:存放要写入的数据的缓存区指针
		wr_add:开始写入的地址
		wr_size:写入的数据大小
    *函数功能	CPU通过串口指定地址写数据
    *返回值	
****************************************************************/
uint8_t EE_writebits( uint8_t *wr_data , uint16_t wr_add , uint16_t wr_size )
{
	uint16_t m,i; 							//数据写入地址为首页地址时，用于重新发送地址所需用到的变量
	uint16_t w_add;
	w_add = wr_add ; 
	for( i=0 ; i<wr_size ; i++ )					//需要写入的大小
	{
		if(( i==0 )||( w_add & (EE_PAGE-1))== 0)		        //当写入第一个字节或者写入到新的一页的首地址时，需要重新发送地址
		{
			i2c_end() ;					//先发送结束信号结束传输
			for( m=0 ; m<1000 ; m++)			//开始发送
			{
				i2c_start() ; 				//发送启动信号
				i2c_writebits( EE_ADD | I2C_WR) ; 	//发送设备地址，此处是写信号  
				if(i2c_waitack() == 0)			//检测应答信号
				{
					break ;				//已检测到应答信号，跳出循环
				}
			}
			if(m == 1000)					//循环发送地址都没有接收到应答信号
			{
				goto fail ; 				//未收到应答信号，结束传输
			}
			i2c_writebits((uint8_t)w_add) ;			// 收到应答信号，发送开始写入的地址
			if(i2c_waitack()!=0)				//检测是否收到应答信号
			{
				goto fail ; 				//未收到应答信号，结束传输
			}	
		}
		i2c_writebits(wr_data[i]) ; 				//前面的代码实现好地址的写入后，开始传输数据
		if(i2c_waitack()!=0)					//检测应答信号
		{
			goto fail ; 					//未收到应答信号，结束传输
		}
		w_add++ ; 						//每次写完一个字节，写入地址自加一，以便当前页面写满的时候，新的页面地址写入需要该地址
	}
	i2c_end() ; 							//数据写入完毕，发送结束信号
	return 1 ; 							//写入成功，返回值为1
	
	fail:	i2c_end() ;						// 写入失败，发送结束信号
		return 0 ; 						//写入失败，返回值为0	
}

/***************************************************************
    *函数名：	EE_erase()
    *形参名	
    *函数功能	
    *返回值	
****************************************************************/
void EE_erase(void)
{
	uint16_t i ;
	uint8_t buff[EE_SIZE];
	for( i=0 ; i<EE_SIZE ; i++) 
	{
		buff[i] = 0xff ;
	}
	if(EE_writebits( buff , 0 , EE_SIZE ) == 0 ) 
	{
		printf("Erase EEPRAM fail！\r\n") ; 
		return ; 
	}
	else
	{
		printf("Erase EEPRAM success!\r\n") ; 
	}
}

/***************************************************************
    *函数名：	delay()
    *形参名	
    *函数功能	延时函数
    *返回值	
****************************************************************/
void delay(__IO uint32_t d)
{
	for(; d!=0 ; d--) ; 
}

/***************************************************************
    *函数名：	EE_test()
    *形参名	
    *函数功能	测试读写操作
    *返回值	
****************************************************************/
void EE_test(void)
{
	uint16_t i,j ;
	uint8_t write_buff[EE_SIZE],read_buff[EE_SIZE] ;//定义读写操作时，存放数据所需用到的缓存区 
	//EE_erase() ;
	/*	CPU发送地址信号检测EEPRAM设备	*/
	if(EE_check()==0)
	{
		printf(" Check EEPRAM fail \r\n") ; 
		return ;
	}
	else
	{
		printf(" Check EEPRAM success \r\n") ;
	}
	
	/*	给要写入的缓存区赋值	*/
	for( i=0 ; i<EE_SIZE ; i++ ) 
	{
		write_buff[i] = i ;
		read_buff[i] = 0 ;
	}
	
	
	
	/*	显示要写入的数据	*/
	printf("\r\n The write data are : \r\n") ;
	for( i=0 ; i<EE_SIZE ; i++ ) 
	{
		printf("%02X ",write_buff[i]) ;
	}
	
	printf("\r\n Now,The read data are : \r\n") ;
	for( i=0 ; i<EE_SIZE ; i++ ) 
	{
		printf("%02X ",read_buff[i]) ;
	}
	
	/*	将数据写入缓存区	*/
	if(EE_writebits( write_buff , 0 , EE_SIZE ) ==0 )
	{
		printf("\r\n Write EEPRAM fail \r\n") ;
		return ;  
	}
	else
	{
		printf("\r\n Write EEPRAM success \r\n") ;
	}
	/*	写完后需要延时一段时间再读取	*/
	delay(0x0fffff) ; 
	
	/*	开始读取数据	*/
	if(EE_readbits( read_buff , 0 , EE_SIZE ) == 0 )
	{
		printf(" Read EEPRAM fail \r\n") ;
		return ; 
	}
	else
	{
		printf(" Read EEPRAM success \r\n") ;
	}
	
	/*	检测读取和写入的数据是够一致	*/
	for(j=0;j<EE_SIZE;j++)
	{
		if(read_buff[j]!=write_buff[j])
		{
			printf("\r\n The write data and read data are different \r\n");
			return ; 
		}
		printf(" %02X",read_buff[j]) ;	
	}
	printf("\r\n Read and Write EEPRAM success \r\n");
	while(1) ; 
}
















