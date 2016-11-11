/******
    *************************************************************************************
    *@file	bsp_i2c_ee.c
    *@author	Bin
    *@brief	EEPROM�����ļ�
    *************************************************************************************
******/

#include "bsp_i2c_ee.h"

/************************************************************
    *��������	EE_check()
    *�β���	��
    *��������	���EEPRAM�Ƿ����
    *����ֵ	��
*************************************************************/
uint8_t EE_check(void)
{
	if(i2c_check_device( EE_ADD ) == 0) 	//����EEPRAM�ĵ�ַ������Ƿ���ڸ��豸
	{
		return 1 ; 			//������ڣ�����1
	}
	else
	{			
		i2c_end() ;			//���ҷ���I2C���߽����ź� 
		return 0 ; 			//��������ڣ�����0
	}
	
}

/***************************************************************
    *��������	EE_readbits()
    *�β���	data:��Ŷ�ȡ�������ݵĻ�����ָ��
		ee_data_add:��ʼ��ȡ�����ݵĵ�ַ
		ee_read_size:��ȡ���ݵĴ�С
    *��������	CPUͨ������ָ����ȡEEPRAM�е�����
    *����ֵ	����ֵΪ0����ʾ��ȡʧ�ܣ�����ֵΪ1����ʾ��ȡ�ɹ�
****************************************************************/
uint8_t EE_readbits( uint8_t *data , uint16_t ee_data_add ,uint16_t ee_read_size )
{
	uint16_t i ; 				//��ΪAT24c02�Ĵ洢��С��256���ֽڣ�����16λ�㹻��ʾ
	i2c_start() ; 				//��ʼ�ź�
	i2c_writebits(EE_ADD | I2C_RD) ;	//�����豸��ַ�Ͷ�ȡ�ź�
	if(i2c_waitack()!=0)			//����Ƿ��ͳɹ�
	{
		goto fail ; 			//δ�յ�Ӧ���źţ���ȡʧ��
	}
	i2c_start() ; 				//�յ�Ӧ���źţ�����������ʼ�ź�
	i2c_writebits(ee_data_add) ;		//���Ͷ�ȡ����ʵ��ַ
	if(i2c_waitack()!=0)			//����Ƿ��ͳɹ�
	{
		goto fail ; 			//I2C�����豸δ����Ӧ���źţ���ȡʧ��
	}
	for( i=0 ; i<ee_read_size ; i++ )	//��ʼ��ȡ����
	{
		data[i] = i2c_readbits() ;	//����ȡ�������ݴ�ŵ����ݻ�����
		if(i<ee_read_size-1)
		{
			i2c_ack() ; 		//δ��ȡ�����ݣ�ÿ�ζ�ȡ��һ���ֽڵ����ݷ���һ��Ӧ���ź�
		}
		else
		{
			i2c_uack() ; 		//�������ݶ�ȡ��Ϻ󣬷��ͷ�Ӧ���ź�
		}
	}
	i2c_end() ; 				//���ͽ����ź�
	return 1 ; 				//����ֵΪ1��˵����ȡ�ɹ�
				
	fail: 	i2c_end() ; 			//��ȡʧ�ܣ����ͽ����ź�
		return 0 ; 			//����ֵΪ0��˵����ȡʧ��
}

/***************************************************************
    *��������	EE_writebits()
    *�β���	
    *��������	CPUͨ������ָ����ַд����
    *����ֵ	
****************************************************************/
uint8_t EE_writebits( uint8_t *wr_data , uint16_t wr_add , uint16_t wr_size )
{
	uint16_t m ; //����д���ַΪ��ҳ��ַʱ���������·��͵�ַ�����õ��ı���
	uint16_t i,w_add;
	for( i=0 ; i<wr_size ; i++ )//��Ҫд��Ĵ�С
	{
		if((i==0) || ( w_add & (EE_PAGE-1)) == 0)//��д���һ���ֽڻ���д�뵽�µ�һҳ���׵�ַʱ����Ҫ���·��͵�ַ
		{
			i2c_end() ;//�ȷ��ͽ����źŽ�������
			for( m=0 ; m<1000 ; m++)//��ʼ����
			{
				i2c_start() ; //���������ź�
				i2c_writebits(EE_ADD | I2C_WR) ; //���͵�ַ��д�ź�
				if(i2c_waitack() == 0)//���Ӧ���ź�
				{
					break ;//�Ѽ�⵽Ӧ���źţ�����ѭ��
				}
			}
			if(m == 1000)//ѭ�����͵�ַ��û�н��յ�Ӧ���ź�
			{
				goto fail ; //δ�յ�Ӧ���źţ���������
			}
			i2c_writebits((uint8_t)w_add) ;// �յ�Ӧ���źţ����Ϳ�ʼд��ĵ�ַ
			if(i2c_waitack()!=0)//����Ƿ��յ�Ӧ���ź�
			{
				goto fail ; //δ�յ�Ӧ���źţ���������
			}	
		}
		i2c_writebits(wr_data[i]) ; //ǰ��Ĵ���ʵ�ֺõ�ַ��д��󣬿�ʼ��������
		if(i2c_waitack()!=0)//���Ӧ���ź�
		{
			goto fail ; //δ�յ�Ӧ���źţ���������
		}
		w_add++ ; 	//ÿ��д��һ���ֽڣ�д���ַ�Լ�һ���Ա㵱ǰҳ��д����ʱ���µ�ҳ���ַд����Ҫ�õ�ַ
	}
	i2c_end() ; //����д����ϣ����ͽ����ź�
	return 1 ; //д��ɹ�������ֵΪ1
	
	fail:	i2c_end() ;// д��ʧ�ܣ����ͽ����ź�
		return 0 ; //д��ʧ�ܣ�����ֵΪ0	
}

/***************************************************************
    *��������	EE_erase()
    *�β���	
    *��������	
    *����ֵ	
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
		printf("����EEPRAMʧ�ܣ�\r\n") ; 
		return ; 
	}
	else
	{
		printf("����EEPRAM�ɹ�\r\n") ; 
	}
}

/***************************************************************
    *��������	delay()
    *�β���	
    *��������	��ʱ����
    *����ֵ	
****************************************************************/
void delay(__IO uint32_t d)
{
	for(; d!=0 ; d--) ; 
}

/***************************************************************
    *��������	EE_test()
    *�β���	
    *��������	
    *����ֵ	
****************************************************************/
void EE_test(void)
{
	uint16_t i,j ;
	uint8_t write_buff[EE_SIZE],read_buff[EE_SIZE] ; 
	if(EE_check()==0)
	{
		printf("Check EEPRAM fail\r\n") ; 
		return ;
	}
	else
	{
		printf("Check EEPRAM success\r\n") ;
	}
	
	for( i=0 ; i<EE_SIZE ; i++ ) 
	{
		write_buff[i] = i ;
	}
	
	if(EE_writebits( write_buff , 0 , EE_SIZE ) ==0 )
	{
		printf("Write EEPRAM fail\r\n") ;
		return ;  
	}
	else
	{
		printf("Write EEPRAM success\r\n") ;
	}
	
	delay(0x0ffffff) ; 
	
	if(EE_readbits( read_buff , 0 , EE_SIZE ) == 0 )
	{
		printf("Read EEPRAM fail\r\n") ;
		return ; 
	}
	else
	{
		printf("Read EEPRAM success\r\n") ;
	}
	for(j=0;j<EE_SIZE;j++)
	{
		if(read_buff[j]!=read_buff[j])
		{
			printf("0x%02X ",write_buff[j]) ;
			printf("The write data and read data are different\r\n");
			return ; 
		}
		printf(" 0x%02X",read_buff[j]) ;
		if((j&15)==15)
		{
			printf("\r\n") ; 
		}	
	}
	printf("Read and Write EEPRAM success\r\n");
	while(1) ; 
}
















