#include "sl_rtc.h"
#include "so_lib.h"


DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);

/*
	使用方法：char *dt = "2017-11-30 18:59:00"
*/
EXPORT int set_rtc_time(int dev_fd, const char *dt)
{
	struct rtc_time rtc_tm;

	int ret = sscanf(dt, "%d-%d-%d %d:%d:%d", &rtc_tm.tm_year, &rtc_tm.tm_mon, \
												&rtc_tm.tm_mday, &rtc_tm.tm_hour, \
												&rtc_tm.tm_min, &rtc_tm.tm_sec);
	if (ret != 6) {
		ERR("Time not match");
		return -1;
	}
	/*  rtc内核源码摘录
		mon = rtc_tm.tm_mon + 1;    tm_mon starts at zero 
	*/
	rtc_tm.tm_mon -= 1;
	ret = ioctl(dev_fd, RTC_SET_TIME, &rtc_tm);
	if (ret == -1) {
		ERR("ioctl RTC_RD_TIME");
	}
	return ret;
}

EXPORT int get_rtc_time(int dev_fd, struct rtc_time *rtc_tm)
{
	if (rtc_tm == NULL)
		return -1;
	struct rtc_time tmp;
	memset(&tmp, 0, sizeof(struct rtc_time));
	int ret = ioctl(dev_fd, RTC_RD_TIME, &tmp);
	if (ret == -1) {
		ERR("ioctl RTC_RD_TIME");
		return -1;
	}
	tmp.tm_year += 1900;
	tmp.tm_mon++;
	memcpy(rtc_tm, &tmp, sizeof(struct rtc_time));
	return ret;
}


/*
	闹钟只能设置小时，分钟和秒， 既闹钟时间不能超过24h
	在驱动中只读取了小时分钟和秒，所以就不用设置年月日了
	char *dt = "19:33:00"
*/
EXPORT int set_alm_time(int dev_fd, const char *dt)
{
	struct rtc_time rtc_tm;
	int ret = sscanf(dt , "%d:%d:%d", &rtc_tm.tm_hour, &rtc_tm.tm_min, &rtc_tm.tm_sec);
	if (ret != 3) {
		ERR("alm not match");
		return -1;
	}
	ret = ioctl(dev_fd, RTC_ALM_SET, &rtc_tm);
	if (ret == -1) {
		ERR("ioctl RTC_ALM_SET");
	}
	return ret;
}


/* 只读取小时、分钟、秒 */
EXPORT int get_alm_time(int dev_fd, struct rtc_time *rtc_tm)
{
	if (rtc_tm == NULL)
		return -1;
	memset(rtc_tm, 0, sizeof(struct rtc_time));
	int ret = ioctl(dev_fd, RTC_ALM_READ, rtc_tm);
	if (ret == -1) {
		ERR("ioctl RTC_RD_TIME");
	}
	return ret;
}


EXPORT int alarm_interrupt(int dev_fd, unsigned char irq_state)
{
	if (irq_state != DISABLE) {
		return ioctl(dev_fd, RTC_AIE_ON, 0);
	} else {
		return ioctl(dev_fd, RTC_AIE_OFF, 0);
	}	
}

EXPORT int update_interrupt(int dev_fd, unsigned char irq_state)
{
	if (irq_state != DISABLE) {
		return ioctl(dev_fd, RTC_UIE_ON, 0);
	} else {
		return ioctl(dev_fd, RTC_UIE_OFF, 0);
	}	
}


EXPORT int read_interrupt_flag(int dev_fd)
{
	unsigned char flag;
	return read_hardware(dev_fd, &flag, 1);
}

EXPORT int is_periodic_irq(unsigned char value)
{
	return value & RTC_PF ? 0 : -1;
}

EXPORT int is_larm_irq(unsigned char value)
{
	return value & RTC_AF ? 0 : -1;
}

EXPORT int is_update_irq(unsigned char value)
{
	return value & RTC_UF ? 0 : -1;
}
