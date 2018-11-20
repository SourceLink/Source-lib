#include "bsp_can.h"
#include "sl_perpheral.h"
#include "so_lib.h"

DEBUG_SET_LEVEL(DEBUG_LEVEL_DEBUG);


unsigned char send_buf[10] = {0xA5, 0xB4, 0xC3, 0xD2};
unsigned char rev_buf[10] = {0};
int rev_can_id = 0;

int main(int argc, char** argv)
{
	pid_t pid = -1;
	int master_fd;
	int slave_fd;
	master_fd = can_init(CAN0, 1000000, 0xA5A6, EXTENDED_FRAME);
	slave_fd = can_init(CAN1, 1000000, 0xA5A6, EXTENDED_FRAME);

	if (master_fd == -1 || slave_fd == -1) {
		ERR("get fd\n");
		return -1;
	}
	
	 pid = fork();
	 if (pid == -1) {
		ERR("fork\n");
	 } else if (pid == 0) {/* 子进程 */
		while (1) {
			can_send_data(master_fd, send_buf, 4);
			sleep(1);
		}
	 } else {
		while (1) {
			rev_can_id = read_frame(slave_fd, rev_buf, 4);
			//DEBUG("rev_can_id= %x\n", rev_can_id);
			/* 扩展帧最高位为1，定义的又是无符号型所以会被识别为负数 */
			if (rev_can_id != -1) {
				INFO("rev_can_id = %x\n", rev_can_id);
				INFO("rev_buf: %x %x %x %x\n", rev_buf[0], rev_buf[1], rev_buf[2], rev_buf[3]);
			}
			sleep(1);
		}
	 }
	return 0;
}
