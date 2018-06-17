#include "gpio_iic.h"

void GPIO_IIC_config( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure );
}


void delay_5us( u32 nTime )     /* delay_10us(1)  //5us half T */
{
	int i, j;
	for ( i = 0; i < nTime; i++ )
	{
		for ( j = 0; j < 500; j++ )
		{
			;
		}
	}
}


void delay_ms( u32 nTime )
{
	int i, j;
	for ( i = 0; i < nTime; i++ )
	{
		for ( j = 0; j < 72000; j++ )
		{
			;
		}
	}
}


void iic_start( void )
{
	SCL_L
	delay_5us( 1 );


	SDA_H
	delay_5us( 1 );


	SCL_H
	delay_5us( 1 );


	SDA_L
	delay_5us( 1 );
}


void iic_stop( void )
{
	SCL_L
	delay_5us( 1 );


	SDA_L
	delay_5us( 1 );


	SCL_H
	delay_5us( 1 );


	SDA_H
	delay_5us( 1 );
}


u8 iic_recive_ack( void )
{
	u8 recive_ack = 0x11;
	SCL_L
	delay_5us( 1 );


	SDA_H
	delay_5us( 1 );


	SCL_H
	delay_5us( 1 );


	recive_ack = GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_7 );
	return(recive_ack);
}


void iic_send_ack( u8 ack )
{
	u8 send_ack;
	send_ack = (ack) ? 1 : 0;
	if ( send_ack )
	{
		SDA_H
		delay_5us( 1 );
	}else{
		SDA_L
		delay_5us( 1 );
	}
	SCL_H
	delay_5us( 1 );


	SCL_L
	delay_5us( 1 );
}


u8 iic_write_byte( u8 senddata )
{
	int	i;
	u8	status;
	u8	temp_data;
	for ( i = 0; i < 8; i++ )
	{
		temp_data	= senddata & 0x80;
		senddata	<<= 1;
		temp_data	= (temp_data) ? 1 : 0;
		SCL_L
		delay_5us( 1 );


		if ( temp_data )
		{
			SDA_H
			delay_5us( 1 );
		}else{
			SDA_L
			delay_5us( 1 );
		}
		SCL_H
		delay_5us( 1 );
	}
	status = iic_recive_ack();
	return(status);
}


u8 iic_recive_byte( void )
{
	int	i;
	u8	recive_byte = 0;
	SCL_L
	delay_5us( 1 );


	SDA_H
	delay_5us( 1 );


	for ( i = 0; i < 8; i++ )
	{
		recive_byte <<= 1;
		SCL_H
		delay_5us( 1 );


		recive_byte |= GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_7 );
		SCL_L
		delay_5us( 1 );
	}
	return(recive_byte);
}


void Write_Byte( u8 DeviceAddr, u8 WriteAddr, u8 data )
{
	iic_start();
	iic_write_byte( DeviceAddr );
	iic_write_byte( WriteAddr );
	iic_write_byte( data );
	iic_stop();
}


u8 Read_Byte( u8 DeviceAddr, u8 ReadAddr )
{
	u8 returnByte;
	iic_start();
	iic_write_byte( DeviceAddr );
	iic_write_byte( ReadAddr );
	iic_start();
	iic_write_byte( DeviceAddr | 0x01 );
	returnByte = iic_recive_byte();
	iic_send_ack( 1 );
	iic_stop();
	return(returnByte);
}


void Read_ByteBuffer( u8 DeviceAddr, u8 ReadAddr, u8 ReadBuffer[], u32 size )
{
	int i;
	iic_start();
	iic_write_byte( DeviceAddr );
	iic_write_byte( ReadAddr );
	iic_start();
	iic_write_byte( DeviceAddr | 0x01 );
	for ( i = 0; i < size - 1; i++ )
	{
		ReadBuffer[i] = iic_recive_byte();
		iic_send_ack( 0 );
	}
	ReadBuffer[i] = iic_recive_byte();
	iic_send_ack( 1 );
	iic_stop();
}


int Sensors_I2C_WriteRegister( unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, const unsigned char *RegisterValue )
{
	u8	status = 0;
	int	i;
	iic_start();
	status	|= iic_write_byte( Address );
	status	|= iic_write_byte( RegisterAddr );
	for ( i = 0; i < RegisterLen; i++ )
	{
		status |= iic_write_byte( RegisterValue[i] );
	}
	iic_stop();
	return(status);
}


int Sensors_I2C_ReadRegister( unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, unsigned char *RegisterValue )
{
	u8	status = 0;
	int	i;
	if ( RegisterLen == 1 )
	{
		iic_start();
		status	|= iic_write_byte( Address );
		status	|= iic_write_byte( RegisterAddr );
		iic_start();
		status			|= iic_write_byte( Address | 0x01 );
		RegisterValue[0]	= iic_recive_byte();
		iic_send_ack( 1 );
		iic_stop();
		return(status);
	}else{
		iic_start();
		status	|= iic_write_byte( Address );
		status	|= iic_write_byte( RegisterAddr );
		iic_start();
		status |= iic_write_byte( Address | 0x01 );
		for ( i = 0; i < RegisterLen - 1; i++ )
		{
			RegisterValue[i] = iic_recive_byte();
			iic_send_ack( 0 );
		}
		RegisterValue[i] = iic_recive_byte();
		iic_send_ack( 1 );
		iic_stop();
		return(status);
	}
	return(1);
}


