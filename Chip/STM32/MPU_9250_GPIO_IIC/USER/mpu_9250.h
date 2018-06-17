#ifndef __MPU_9250_H
#define __MPU_9250_H

#define GYR_ACC_DEVICE 	0xD0
#define MAG_DEVICE     	0x18
#define MAG_ID_ADDR     0x00
#define MAG_CTR1_ADDR   0x0A
#define MAG_CTR2_ADDR   0x0B
#define MAG_STATUS_1    0x02
#define MAG_STATUS_2    0x09
#define USER_CTRL      	0x6A
#define PWR_MAGMT_1    	0x6B
#define WHO_AM_I       	0x75
#define SIMPLE_DIV			0x19    //·ÖÆµ   SAMPLE_RATE=Internal_Sample_Rate / (1 + SMPLRT_DIV)
#define CONFIG         	0x1A
#define CONFIG_GYRO    	0x1B
#define CONFIG_ACCL_1  	0x1C
#define CONFIG_ACCL_2  	0x1D
#define INT_PIN_CONFIG  0x37
#define INT_ENABLE     	0x38
#define INT_STATUS     	0x3A
#define ACCL_REGISTER   0x3B    //ACCL-TEMP-GRYO   (14H-L)
#define MAG_REGISTER    0x03    //MAG  (6L-h)
#define MAG_ASAX        0x10
#define MAG_ASAY        0x11
#define MAG_ASAZ        0x12
#define MAG_XOUT_L      0x03
#define MAG_XOUT_H      0x04
#define MAG_YOUT_L      0x05
#define MAG_YOUT_H      0x06
#define MAG_ZOUT_L      0x07
#define MAG_ZOUT_H      0x08

#endif

