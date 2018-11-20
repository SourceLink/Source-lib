#include "bsp_mpu6050.h"
#include "sl_perpheral.h"
#include "so_lib.h"

DEBUG_SET_LEVEL(DEBUG_LEVEL_DEBUG);

int main(int argc, char ** argv)
{
	int dev_fd;
	if ((dev_fd = mpu6050_init()) < 0) {
		ERR("Fail mpu6050_init\n");
		return -1;
	} else {
		INFO("Successful mpu6050_init\n");
	}
	while (1) {
			if (mpu6050_read_data(dev_fd)) {
				ERR("mpu6050_read_data fail!!\n");
			} else {
				INFO("DIRECTION_X angle: %f\n", accel_to_angle(DIRECTION_X));
			}
			sleep(1);
			/* i2c1设置和串口4冲突 UART4_TX_DATA  */
	}
	return 0;
}
