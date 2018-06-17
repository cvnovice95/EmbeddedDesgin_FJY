#include "mpu.h"
#include <math.h>
int main()
{
	float	gx, gy, gz, ax, ay, az, mx, my, mz, hx, hy, hz, nhy, nhx;
	float	OffsetX, OffsetY, OffsetZ;
	AXIS	q_angle, hmc;
	double	angle;
	GPIO_IIC_config();
	UART_config();
	MPU_9250_Init();
	/* MPU_DMP_Init(); */
	HMC5883L_Init();
	AdjustMag( &OffsetX, &OffsetY, &OffsetZ );
	while ( 1 )
	{
		Read_HMC( &hmc, &hx, &hy, &hz );
		/*
		 * angle = atan2((double)(hy-OffsetY),(double)(hx-OffsetX)) * (180 / 3.14159265) + 180;
		 * AHRSupdate(gx,gy,gz,ax,ay,az,&q_angle);
		 */
		MPU_9250_Raw( &gx, &gy, &gz, &ax, &ay, &az, &mx, &my, &mz );
		AHRSupdate_m( gx, gy, gz, ax, ay, az, mx, my, mz, &q_angle );
		/*
		 * AHRSupdate_m(gx,gy,gz,ax,ay,az,hx,hy,hz,&q_angle);
		 * DMP
		 * Read_DMP(&q_angle);
		 */
		nhy = hy * cos( q_angle.X ) + hx*						sin( q_angle.X ) * sin( q_angle.Y ) - hz*cos( q_angle.Y ) * sin( q_angle.X );


		nhx = hx * cos( q_angle.Y ) + hz*sin( q_angle.Y );


		angle = atan2( (double) (nhy - OffsetY), (double) (nhx - OffsetX) ) * (180 / 3.14159265) + 180;
		printf( "\r\n Yaw=%.3f   ,Roll=%.3f    ,Pitch=%.3f \r\n", (float) angle, (float) q_angle.Y, (float) q_angle.X );
		printf( "\r\n <----------------------------------------> \r\n" );

		/*
		 * Read_HMC(&hmc,&hx,&hy,&hz);
		 * angle = atan2((double)(hy-OffsetY),(double)(hx-OffsetX)) * (180 / 3.14159265) + 180;
		 * printf("\r\n HX=%.3f   ,HY=%.3f    ,HZ=%.3f \r\n",hmc.X,hmc.Y,hmc.Z);
		 * printf("\r\n X=%.3f   ,Y=%.3f    ,Z=%.3f   angle=%.3f \r\n",hx,hy,hz,angle);
		 * printf("\r\n <----------------------------------------> \r\n");
		 */
	}
	return(0);
}