#include "bsp_mpu6050.h"
#include "sl_i2c.h"
#include "so_lib.h"

DEBUG_SET_LEVEL(DEBUG_LEVEL_INFO);

static mpu6050_typedef g_mpu6050;				/* 定义一个全局变量，保存实时数据 */

EXPORT int mpu6050_init(void)
{
	int dev_fd;
	if ((dev_fd = open_hardware(I2C1_PATH, O_RDWR)) < 0) {
		ERR("Fail to open I2C device\n");
		return -1;
	} 
	if (set_i2c_slave(dev_fd, MPU6050_SLAVE_ADDRESS) < 0) {
		ERR("Fail to set mpu6050 slave address\n");
		close_hardware(dev_fd);
		return -1;
	}
	i2c_write_byte_data(dev_fd, PWR_MGMT_1, 0x00);
	i2c_write_byte_data(dev_fd, SMPLRT_DIV, 0x07);
	i2c_write_byte_data(dev_fd, LRF_CONFIG, 0x06);
	i2c_write_byte_data(dev_fd, GYRO_CONFIG, 0xE8);
	i2c_write_byte_data(dev_fd, ACCEL_CONFIG, 0x01);
	return dev_fd;
}

EXPORT void mpu6050_deinit(int dev_fd)
{
	close_hardware(dev_fd);
}

EXPORT int mpu6050_read_data(int dev_fd)
{
	int ret = 0;
	int i;
	unsigned char temp_buf[20];
	for (i = 0; i < 14; i++) {
		temp_buf[i] = i2c_read_byte_data(dev_fd, ACCEL_XOUT_H + i);
		if (temp_buf[i] == -1)
			return -1;
	}
	/* 将读出的数据保存到全局结构体变量 */
	/* 加速度角度数据 */
	g_mpu6050.Accel_X = (temp_buf[0] << 8) + temp_buf[1];
	g_mpu6050.Accel_Y = (temp_buf[2] << 8) + temp_buf[3];
	g_mpu6050.Accel_Z = (temp_buf[4] << 8) + temp_buf[5];
	/* 温度 */
	g_mpu6050.Temp = (short)((temp_buf[6] << 8) + temp_buf[7]);
	/* 陀螺仪角速度数据 */
	g_mpu6050.GYRO_X = (temp_buf[8] << 8) + temp_buf[9];
	g_mpu6050.GYRO_Y = (temp_buf[10] << 8) + temp_buf[11];
	g_mpu6050.GYRO_Z = (temp_buf[12] << 8) + temp_buf[13];
	return ret;
}

/*
*********************************************************************************************************
*	函 数 名: accel_to_angle_x
*	功能说明: 根据加速度值获取角度值  与x轴方向的夹角
*	形    参: mpu6050_des:从mpu6050中读取到的数据
*	返 回 值: 角度值
*********************************************************************************************************
*/
static float accel_to_angle_x(mpu6050_typedef *mpu6050_des)
{
	float temp_angle = 0.0;
	temp_angle = atan(mpu6050_des->Accel_X / sqrt(mpu6050_des->Accel_Y * mpu6050_des->Accel_Y + mpu6050_des->Accel_Z * mpu6050_des->Accel_Z)) * 57.2958;																				
	return temp_angle;
}


/*
*********************************************************************************************************
*	函 数 名: accel_to_angle_y
*	功能说明: 根据加速度值获取角度值  与y轴方向的夹角
*	形    参: mpu6050_des:从mpu6050中读取到的数据
*	返 回 值: 角度值
*********************************************************************************************************
*/
static float accel_to_angle_y(mpu6050_typedef *mpu6050_des)
{
	float temp_angle = 0.0;
	temp_angle = atan(mpu6050_des->Accel_Y / sqrt(mpu6050_des->Accel_X * mpu6050_des->Accel_X + mpu6050_des->Accel_Z * mpu6050_des->Accel_Z)) * 57.2958;
	return temp_angle;
}




/*
*********************************************************************************************************
*	函 数 名: accel_to_angle_z
*	功能说明: 根据加速度值获取角度值  与z轴方向的夹角
*	形    参: mpu6050_des:从mpu6050中读取到的数据
*	返 回 值: 角度值
*********************************************************************************************************
*/
static float accel_to_angle_z(mpu6050_typedef *mpu6050_des)
{
	float temp_angle = 0.0;
	temp_angle = atan(mpu6050_des->Accel_Z / sqrt(mpu6050_des->Accel_Y * mpu6050_des->Accel_Y + mpu6050_des->Accel_X * mpu6050_des->Accel_X)) * 57.2958;
	return temp_angle;
}



/*
*********************************************************************************************************
*	函 数 名: accel_to_angle
*	功能说明: 根据加速度值获取角度值
*	形    参: direction: 选择哪个方向的角度 取值:DIRECTION_X、 DIRECTION_Y 、DIRECTION_Z
*	返 回 值: 选择方向的角度值
*********************************************************************************************************
*/
EXPORT float accel_to_angle(unsigned char direction)
{
	float temp_angle = 0.0;
	switch (direction) {
		case DIRECTION_X:
			temp_angle = accel_to_angle_x(&g_mpu6050);
			break;
		case DIRECTION_Y:
			temp_angle = accel_to_angle_y(&g_mpu6050);
			break;
		case DIRECTION_Z:
			temp_angle = accel_to_angle_z(&g_mpu6050);
			break;
		default : break;
	}
	return temp_angle;
}


	
/*
*********************************************************************************************************
*	函 数 名: get_gyro
*	功能说明: 获取角速度值。陀螺仪的灵敏度为16.40 陀螺仪的数据除以16.40 就可以换算成角度值  0.060976= 1/16.40
*	形	  参: directio:选择方向的角速度值 DIRECTION_X、DIRECTION_Y、DIRECTION_Z
*	返 回 值: 无
*********************************************************************************************************
*/
EXPORT float get_gyro(unsigned char directio)
{
	short gyro_value = 0;
	switch (directio) {
		case DIRECTION_X:
			gyro_value = g_mpu6050.GYRO_X;
			break;		
		case DIRECTION_Y:
			gyro_value = g_mpu6050.GYRO_Y;
			break;			
		case DIRECTION_Z:
			gyro_value = g_mpu6050.GYRO_Z;
			break;
	}	
	return (float)(gyro_value * GYRO_SCALE);
}

