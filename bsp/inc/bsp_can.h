#ifndef BSP_CAN_H_
#define BSP_CAN_H_

extern int can_init(int port, 
					int bitrate, 
					int can_id, 
					int frame_type
					);
extern void can_deinit(int socket_fd, int port);
extern int can_send_data(int socket_fd, unsigned char *data_buf, int len);


#endif
