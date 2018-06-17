#ifndef __MPU_H
#define __MPU_H

/* MPU_9250 */
#define GYR_ACC_DEVICE	0xD0
#define MAG_DEVICE	0x18
#define MAG_ID_ADDR	0x00
#define MAG_CTR1_ADDR	0x0A
#define MAG_CTR2_ADDR	0x0B
#define MAG_STATUS_1	0x02
#define MAG_STATUS_2	0x09
#define USER_CTRL	0x6A
#define PWR_MAGMT_1	0x6B
#define WHO_AM_I	0x75
#define SIMPLE_DIV	0x19 /* float exInt = 0, eyInt = 0, ezInt = 0;SAMPLE_RATE=Internal_Sample_Rate / (1 + SMPLRT_DIV) */
#define CONFIG		0x1A
#define CONFIG_GYRO	0x1B
#define CONFIG_ACCL_1	0x1C
#define CONFIG_ACCL_2	0x1D
#define INT_PIN_CONFIG	0x37
#define INT_ENABLE	0x38
#define INT_STATUS	0x3A
#define ACCL_REGISTER	0x3B    /* ACCL-TEMP-GRYO   (14H-L) */
#define MAG_REGISTER	0x03    /* MAG  (6L-h) */
#define MAG_ASAX	0x10
#define MAG_ASAY	0x11
#define MAG_ASAZ	0x12
#define MAG_XOUT_L	0x03
#define MAG_XOUT_H	0x04
#define MAG_YOUT_L	0x05
#define MAG_YOUT_H	0x06
#define MAG_ZOUT_L	0x07
#define MAG_ZOUT_H	0x08
typedef struct {
	short	X;
	short	Y;
	short	Z;
}AXIS;

void MPU_9250_Init();


int MPU_DMP_Init();


void HMC5883L_Init();


void AdjustMag( float * offsetx, float * offsety, float * offsetz );


void Read_DMP( AXIS *q_angle );


void Read_HMC( AXIS * HMC, float* hx, float*hy, float*hz );


void AHRSupdate( float gx, float gy, float gz, float ax, float ay, float az, AXIS *q_angle );


void AHRSupdate_m( float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, AXIS *q_angle );


void MPU_9250_Raw( float* gx, float*gy, float*gz, float*ax, float*ay, float*az, float*mx, float*my, float*mz );


/* MPU_6050 */


/* HMC5883L */
#define HMC_DEVICE	0x3c
#define HMC_CRA		0x00
#define HMC_CRB		0x01
#define HMC_MODE	0x02
#define HMC_MGA_X_H	0x03
#define HMC_MGA_X_L	0x04
#define HMC_MGA_Z_H	0x05
#define HMC_MGA_Z_L	0x06
#define HMC_MGA_Y_H	0x07
#define HMC_MGA_Y_L	0x08
#define HMC_VA		0x0a /* Value 0x33 */

#endif