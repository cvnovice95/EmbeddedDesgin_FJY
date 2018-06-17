#ifndef __GPIO_IIC_H
#define __GPIO_IIC_H
#include "stm32f10x.h"

#define SDA_H	GPIO_SetBits( GPIOB, GPIO_Pin_7 );
#define SDA_L	GPIO_ResetBits( GPIOB, GPIO_Pin_7 );
#define SCL_H	GPIO_SetBits( GPIOB, GPIO_Pin_6 );
#define SCL_L	GPIO_ResetBits( GPIOB, GPIO_Pin_6 );

void delay_5us( u32 nTime );


void delay_ms( u32 nTime );


void GPIO_IIC_config( void );


void Write_Byte( u8 DeviceAddr, u8 WriteAddr, u8 data );


u8 Read_Byte( u8 DeviceAddr, u8 ReadAddr );


void Read_ByteBuffer( u8 DeviceAddr, u8 ReadAddr, u8 ReadBuffer[], u32 size );


int Sensors_I2C_ReadRegister( unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, unsigned char *RegisterValue );


int Sensors_I2C_WriteRegister( unsigned char Address, unsigned char RegisterAddr, unsigned short RegisterLen, const unsigned char *RegisterValue );


#endif