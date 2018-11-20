#include "bsp_oled.h"
#include "sl_perpheral.h"
#include "so_lib.h"

DEBUG_SET_LEVEL(DEBUG_LEVEL_DEBUG);

int main(int argc, char** argv)
{
	int dc_pin = 0, res_pin = 0;
	int dev_fd = 0;

	/* imx系列板子io_port要减1 */
	if ((res_pin = convert_gpio_pin("GPIO4_02")) == -1 || (dc_pin  = convert_gpio_pin("GPIO4_07")) == -1) {
		DEBUG("convert_gpio_pin error\n");
		return -1;
	} else {
		DEBUG("res_pin = %d, dc_pi = %d\n ", res_pin, dc_pin);
	}
	if ((dev_fd = oled_init(dc_pin, res_pin)) == -1) {
		DEBUG("oled_init error!\n");
		return -1;
	}
	oled_clean_screen(dev_fd);
	oled_disp_8x16str(dev_fd, 0, 0, "Sourcelink");
	oled_disp_8x16str(dev_fd, 0, 16, "Sourcelink");
	oled_deinit(dev_fd);
	return 0;	
}



