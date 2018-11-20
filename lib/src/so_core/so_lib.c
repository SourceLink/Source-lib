/*
*********************************************************************************************************
*
*	文件名称 : so_lib.c
*	说    明 : 对接驱动层的操作，如open,read，write,直接对低层进行读写交互
*				
*   作    者 : Sourcelink
*   日    志 : 
*   日    期 : 2017-11-7
*
*********************************************************************************************************
*/
#include "so_lib.h"

/* 设置DEBUG打印级别 */
DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);


EXPORT int open_hardware(const char *dev_name, int flags)
{
	int fd = -1;
	fd = open(dev_name, flags);
	if (fd < 0) {
		ERR("open_hardware %s", dev_name);
	}
	return fd;
}


EXPORT int write_hardware(int dev_fd, const void *data, size_t len)
{
	if (len <= 0) {
		return -1;
	}
	unsigned char *pdata = (unsigned char*)data;
	int ret = write(dev_fd, pdata, len);
	if (ret < 0) {
		ERR("write_hardware %s", pdata);
	}
	return ret;
}


EXPORT int read_hardware(int dev_fd, void *buf, size_t len)
{
	if (len <= 0) {
		return -1;
	}
	int ret = -1;
	unsigned char *pbuf = (unsigned char*)buf;
	if (pbuf) {
		ret = read(dev_fd, pbuf, len);
	}
	/* 在非阻塞模式下，当没有数据时读出来是-1 */
	if (ret < 0) {
		ERR("read_hardware");
	}
	return ret;
}


EXPORT void close_hardware(int dev_fd)
{
	close(dev_fd);
}

