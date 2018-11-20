#include "so_lib.h"
#include "sl_perpheral.h"
#include "bsp_rtc.h"

DEBUG_SET_LEVEL(DEBUG_LEVEL_INFO);


int main(int argc, char** argv)
{
	int dev_fd;
	struct rtc_time tm;
	memset(&tm, 0, sizeof(struct rtc_time));
	dev_fd = rtc_init_time("2017-11-30 21:20:00");
	sleep(1);
	rtc_get_time(dev_fd, &tm);
	INFO("%d-%d-%d %d:%d:%d", tm.tm_year, tm.tm_mon, \
							tm.tm_mday, tm.tm_hour, \
							tm.tm_min, tm.tm_sec);
	rtc_deinit(dev_fd);
	
	return 0;
}
