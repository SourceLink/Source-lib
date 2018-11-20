#include "bsp_can.h"
#include "sl_can.h"
#include "so_lib.h"


DEBUG_SET_LEVEL(DEBUG_LEVEL_DEBUG);

static int g_can_id = 0;
static int g_frame_type = 0;

/* CAN0 发送  CAN1 接收 */
EXPORT int can_init(int port, 
					int bitrate, 
					int can_id, 
					int frame_type
					)
{
	int socket_fd = -1;
	close_can(port);
	set_bitrate(port, bitrate);
	open_can(port);
	socket_fd = socket_can_listen(port);
	if (socket_fd < 0) {
		ERR("socket_can_listen\n");
		return -1;
	}
	set_can_filter(socket_fd, can_id, frame_type); 
	g_can_id = can_id;
	g_frame_type = frame_type;
	return socket_fd;
}

EXPORT int can_send_data(int socket_fd, unsigned char *data_buf, int len)
{
	return send_frame(socket_fd, g_can_id, g_frame_type, data_buf, len);
}

EXPORT void can_deinit(int socket_fd, int port)
{
	socket_can_close(socket_fd);
	close_can(port);
}

