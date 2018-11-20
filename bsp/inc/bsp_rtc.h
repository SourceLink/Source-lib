#ifndef BSP_RTC_H_
#define BSP_RTC_H_

#include "sl_rtc.h"

extern int rtc_init_time(const char *dt);
extern int rtc_get_time(int dev_fd, struct rtc_time *tm);
extern void rtc_deinit(int dev_fd);


#define RTC_PATH "/dev/rtc"

#endif
