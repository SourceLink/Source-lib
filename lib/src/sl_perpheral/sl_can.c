#include "sl_can.h"
#include "so_lib.h"
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/if.h>

DEBUG_SET_LEVEL(DEBUG_LEVEL_INFO);

static int get_can_port(int port)
{
	int temp_port = -1;
	switch (port) {
		case CAN0:	temp_port = 0; break;
		case CAN1:	temp_port = 1; break;
		default: 
			ERR("Please enter the correct number for can");
			return -1;
	}
	return temp_port;
}

EXPORT int open_can(int port)
{
	char tmep_buf[50];
	int temp_port = -1;
	if ((temp_port = get_can_port(port)) == -1)
		return -1;
	sprintf(tmep_buf, "ifconfig can%d up", temp_port);
	system(tmep_buf);
	return 0;
}

EXPORT int close_can(int port)
{
	char temp_buf[64];
	int temp_port = -1;
	if ((temp_port = get_can_port(port)) == -1)
		return -1;
	sprintf(temp_buf, "ifconfig can%d down", temp_port);
	system(temp_buf);
	return 0;
}


/* 配置baterate */
EXPORT int set_bitrate(int port, int bitrate)
{
	char temp_buf[128];
	int temp_port = -1;
	if ((temp_port = get_can_port(port)) == -1)
		return -1;	
	sprintf(temp_buf, "ip link set can%d type can bitrate %d triple-sampling on", temp_port, bitrate);
	system(temp_buf);
	return 0;
}

void socket_can_close(int socket_fd)
{
	close(socket_fd);
}

EXPORT int socket_can_listen(int port)
{
	int temp_port = -1;
	if ((temp_port = get_can_port(port)) == -1)
		return -1;
	int socket_fd = 0;
	struct sockaddr_can addr;
    struct ifreq ifr;
	char buf[128];	
	/* 建立套接字，设置为原始套接字，原始CAN协议 */
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (socket_fd < 0) {
		ERR("Fail to get a socket");
		return -1;
	}
	sprintf(buf, "can%d", temp_port);
	strcpy(ifr.ifr_name, buf);
	if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
		goto err;
		ERR("Fail to ioctl can name");
	}
	/* 设置CAN协议 */
	addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
	/* 把socket绑定到can上 */
	if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		goto err;
		ERR("Fail to bind socket_can");
	}
	return socket_fd;
	
err:
	socket_can_close(socket_fd);
	return -1;
}

EXPORT int set_can_filter(int socket_fd, int can_id, int frame_type)
{
	/* 过滤：
		struct can_filter {
            canid_t can_id;
            canid_t can_mask;
   		 };
		公式： <received_can_id> & mask == can_id & mask 
	*/
	struct can_filter rfilter;
	switch (frame_type) {
		case STANDRAD_FRAME: 
			rfilter.can_id   = can_id;
			rfilter.can_mask = CAN_SFF_MASK;
			break;
		case EXTENDED_FRAME: 
			rfilter.can_id	 = can_id | CAN_EFF_FLAG;
			rfilter.can_mask = CAN_EFF_FLAG | CAN_EFF_MASK;
			break;
		case REMOTE_FRAME: 
			rfilter.can_id	 = can_id | CAN_RTR_FLAG;
			rfilter.can_mask = CAN_RTR_FLAG | CAN_EFF_MASK;
			break;
		default :
			return setsockopt(socket_fd, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
	}
	return setsockopt(socket_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
}

EXPORT int set_can_err_filter(int socket_fd, unsigned int err_mask)
{
	unsigned int temp_mask = err_mask;
    return setsockopt(socket_fd, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &temp_mask, sizeof(err_mask));
}

/* mode : disable or enable */
EXPORT int set_can_loopback(int socket_fd, int mode)
{
	/* 在默认情况下，本地回环功能是开启的 */
	int loop_back = mode;
	/* 0表示关闭, 1表示开启(默认)           */
	return setsockopt(socket_fd, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loop_back, sizeof(loop_back));
}

/*
	struct can_frame {
	canid_t can_id;  // 32 bit CAN_ID + EFF/RTR/ERR flags 
	__u8    can_dlc; // frame payload length in byte (0 .. CAN_MAX_DLEN) 
	__u8    data[CAN_MAX_DLEN] __attribute__((aligned(8)));
};
*/
EXPORT int send_frame(int socket_fd,
						int can_id,
						int frame_type,	
						unsigned char *data, 
						int len
						)
{
	/* 默认为标准帧发送 */
	struct can_frame frame;
	switch (frame_type) {
		case EXTENDED_FRAME:
			frame.can_id = (can_id & CAN_EFF_MASK) | CAN_EFF_FLAG;
			break;
		case REMOTE_FRAME:
			frame.can_id = (can_id & CAN_EFF_MASK) | CAN_RTR_FLAG;
			break;
		case STANDRAD_FRAME:
		default :
			frame.can_id = can_id & CAN_SFF_MASK;	
		 break;
	}
	
	frame.can_dlc = len;
	memcpy(frame.data, data, len); 
	return write_hardware(socket_fd, &frame, sizeof(struct can_frame));	
}


EXPORT int read_frame(int socket_fd, unsigned char *rev_buf, int len)
{
	struct can_frame frame;
	int ret = read_hardware(socket_fd, &frame, sizeof(struct can_frame));
	if (ret < 0) {
		DEBUG("can raw socket read");
		return -1;
	}
	memcpy(rev_buf, frame.data, len);
	return frame.can_id;
}

