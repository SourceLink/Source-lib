#ifndef BSP_MPU6050_H_
#define BSP_MPU6050_H_

#define MPU6050_SLAVE_ADDRESS    0x68		/* I2C从机地址 */

//****************************************
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	LRF_CONFIG		0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)

#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)


#define DIRECTION_X		(1)
#define DIRECTION_Y		(2)
#define DIRECTION_Z		(3)

#define	GYRO_SCALE 		0.06097609f  //陀螺仪的灵敏度为16.40 陀螺仪的数据除以16.40 就可以换算成角度值  0.060976= 1/16.40


typedef struct
{
	short Accel_X;
	short Accel_Y;
	short Accel_Z;
	short Temp;
	short GYRO_X;
	short GYRO_Y;
	short GYRO_Z;
}mpu6050_typedef;


extern int mpu6050_init(void);
extern void mpu6050_deinit(int dev_fd);
extern int mpu6050_read_data(int dev_fd);
extern float accel_to_angle(unsigned char direction);
extern float get_gyro(unsigned char directio);

#endif