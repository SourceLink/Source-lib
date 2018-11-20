#include "bsp_uart.h"
#include "sl_perpheral.h"
#include "so_lib.h"

DEBUG_SET_LEVEL(DEBUG_LEVEL_DEBUG);

EXPORT int uart_init(const char *dev_name)
{
	int dev_fd;
	if ((dev_fd = open_hardware(dev_name, O_RDWR | O_NONBLOCK)) < 0) {
        DEBUG("Fail to open uart device\n");
        return -1;
    }

	if (uart_setup_port(dev_fd, 115200, 8, 0, 1) < 0) {
		DEBUG("uart_setup_port error\n");
		return -1;
	} else {
		return dev_fd;
	}
}


EXPORT int uart_deinit(int dev_fd)
{
	int ret = 0;
	if ((ret = uart_reset_port(dev_fd)) == -1) {
		return -1;
	}
	close_hardware(dev_fd);
	return ret;
}

