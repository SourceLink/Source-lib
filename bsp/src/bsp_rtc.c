#include "bsp_rtc.h"
#include "so_lib.h"


EXPORT int rtc_init_time(const char *dt)
{
	int dev_fd = -1;
	dev_fd = open_hardware(RTC_PATH, O_RDWR | O_NONBLOCK);
	if (dev_fd == -1)
		return -1;
	if (set_rtc_time(dev_fd, dt) == -1) 
		return -1;
	return  dev_fd;
}

EXPORT int rtc_get_time(int dev_fd, struct rtc_time *tm)
{
	return get_rtc_time(dev_fd, tm);
}


EXPORT void rtc_deinit(int dev_fd)
{
	close_hardware(dev_fd);
}
