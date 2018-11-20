#include "bsp_uart.h"
#include "sl_perpheral.h"
#include "so_lib.h"

DEBUG_SET_LEVEL(DEBUG_LEVEL_DEBUG);


char mesg_buf[] = { 
"			********************************************\r\n\
			A Simple Serial Port Test for sl_lib\r\n\
			********************************************\r\n\
			Author: Sourcelink Email: Sourcelink@126.com\r\n\
			*************************************\r\n\
			databits: 5, 6, 7, 8\r\n\
			parity: 0(None), 1(Odd), 2(Even)\r\n\
			stopbits: 1, 2\r\n\
			Example: %s /dev/ttySAC5 115200 8 0 1\r\n\
			********************************************\r\n\
"
};

int main(int argc, char** argv)
{
	int dev_fd;

	/*
		开启i2c1后，con6会挂掉, 两个外设引脚复用
	*/
	if ((dev_fd = uart_init(CON5)) == -1) {
		DEBUG("uart_init error\n");
		return -1;
	}	
	unsigned char data = 0;
	uart_send_buf(dev_fd, (unsigned char*)mesg_buf, strlen(mesg_buf));
	while (1) {
		if (uart_get_char(dev_fd, &data) > 0)
			uart_send_char(dev_fd, data);
		sleep(1);
		uart_send_char(dev_fd, 'C');
	}
	uart_deinit(dev_fd);
	return 0;
}
