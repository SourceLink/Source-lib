#include "sl_uart.h"
#include <termio.h>
#include "so_lib.h"

DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);

static struct termio g_oldtermio;


static int baudflag_arr[] = {
    B921600, B460800, B230400, B115200, B57600, B38400,
    B19200, B9600, B4800, B2400, B1800, B1200,
    B600, B300, B150, B110, B75, B50
};

static int baud_arr[] = {
    921600, 460800, 230400, 115200, 57600, 38400,
    19200, 9600, 4800, 2400, 1800, 1200,
    600, 300, 150, 110, 75, 50
};

static int baud_convert_to_flag(int baud)
{	
    unsigned int i;
	int ret = B9600;
	for (i = 0; i < ARRAY_SIZE(baud_arr); i++) {
		if (baud == baud_arr[i]) {
			ret = baudflag_arr[i];
			break;
		}
	}
	return ret;
}

static void uart_set_baudrate(struct termio* _termio_attr, int baud)
{
	int baud_flag = baud_convert_to_flag(baud);
	_termio_attr->c_cflag &= ~CBAUD;
	_termio_attr->c_cflag |= baud_flag;
}

static void uart_set_databits(struct termio* _termio_attr, int data_bits)
{
	_termio_attr->c_cflag &= ~(CSIZE);
    switch (data_bits) {
        case 5:
            _termio_attr->c_cflag |= CS5;
            break;

        case 6:
            _termio_attr->c_cflag |= CS6;
            break;

        case 7:
            _termio_attr->c_cflag |= CS7;
            break;

        case 8:
        default:
            _termio_attr->c_cflag |= CS8;
            break;
    }

}

static void uart_set_parity(struct termio* _termio_attr, int parity)
{
	 /* Set parity */
    switch (parity) {
        case 1: /* Odd parity */
            _termio_attr->c_cflag |= (PARENB | PARODD);
            break;

        case 2: /* Even parity */
            _termio_attr->c_cflag |= PARENB;
            _termio_attr->c_cflag &= ~(PARODD);
            break;

        case 0: /* None parity */
        default:
            _termio_attr->c_cflag &= ~(PARENB);
            break;
    }
}


static void uart_set_stopbits(struct termio* _termio_attr, int stop_bits)
{
	 /* set stopbits */
    switch (stop_bits) {
        case 2: /* 2 stopbits */
            _termio_attr->c_cflag |= CSTOPB;
            break;

        case 1: /* 1 stopbits */
        default:
            _termio_attr->c_cflag &= ~CSTOPB;
            break;
    }
}

EXPORT int uart_setup_port(
		int dev_fd, 
		int baud_rate, 
		int data_bits, 
		int parity,
		int stop_bits
		)
{
	struct termio termio_attr;
	int ret = 0;
	/* 获取当前配置 */
	if (ioctl(dev_fd, TCGETA, &termio_attr) < 0) {
		ERR("Can't ioctl TCGETA");
	    return -1;
	}
	/* 备份当前设置 */
	memcpy(&g_oldtermio, &termio_attr, sizeof (struct termio));
	/* 
		IGNBRK 忽略BREAK键输入
		BRKINT 如果设置了IGNBRK，BREAK键输入将被忽略
		IGNPAR 忽略奇偶校验错误
		PARMRK 标识奇偶校验错误
		INPCK  允许输入奇偶校验
		ISTRIP 去除字符的第8个比特
		INLCR  将输入的NL（换行）转换成CR（回车）
		IGNCR  忽略输入的回车
		ICRNL  将输入的回车转化成换行（如果IGNCR未设置的情况下）
		IUCLC  将输入的大写字符转换成小写字符（非POSIX）
		IXON   允许输出时对XON/XOFF流进行控制
		IXANY  输入任何字符将重启停止的输出
		IXOFF  允许输入时对XON/XOFF流进行控制
		IMAXBEL	当输入队列满的时候开始响铃
	*/
	termio_attr.c_iflag &= ~(INLCR | IGNCR | ICRNL | ISTRIP); 
	/* 
		OPOST	处理后输出
		OLCUC   将输入的小写字符转换成大写字符（非POSIX）
		ONLCR   将输入的NL（换行）转换成CR（回车）及NL（换行）
		OCRNL   将输入的CR（回车）转换成NL（换行）
		ONOCR	第一行不输出回车符
		ONLRET	不输出回车符
		OFILL	发送填充字符以延迟终端输出
		OFDEL	以ASCII码的DEL作为填充字符，如果未设置该参数，填充字符为NUL
		NLDLY	换行输出延时，可以取NL0（不延迟）或NL1（延迟0.1s）
		CRDLY	回车延迟，取值范围为：CR0、CR1、CR2和 CR3
		TABDLY	水平制表符输出延迟，取值范围为：TAB0、TAB1、TAB2和TAB3
		BSDLY	空格输出延迟，可以取BS0或BS1
		VTDLY	垂直制表符输出延迟，可以取VT0或VT1
		FFDLY	换页延迟，可以取FF0或FF1
	*/
	termio_attr.c_oflag &= ~(OPOST | ONLCR | OCRNL);
	/*
		ISIG	当输入INTR、QUIT、SUSP或DSUSP时，产生相应的信号
		ICANON	使用标准输入模式
		XCASE	在ICANON和XCASE同时设置的情况下，终端只使用大写。
		ECHO	显示输入字符
		ECHOE	如果ICANON同时设置，ERASE将删除输入的字符
		ECHOK	如果ICANON同时设置，KILL将删除当前行
		ECHONL	如果ICANON同时设置，即使ECHO没有设置依然显示换行符
		ECHOPRT	如果ECHO和ICANON同时设置，将删除打印出的字符（非POSIX）
		TOSTOP	向后台输出发送SIGTTOU信号
	*/
	termio_attr.c_lflag &= ~(ISIG | ECHO | ICANON | NOFLSH);

	uart_set_baudrate(&termio_attr, baud_rate);
	uart_set_databits(&termio_attr, data_bits);
	uart_set_parity(&termio_attr, parity);
	uart_set_stopbits(&termio_attr, stop_bits);

	/*
		VINTR	Interrupt字符
		VEOL	附加的End-of-file字符
		VQUIT	Quit字符	
		VTIME	非规范模式读取时的超时时间
		VERASE	Erase字符	
		VSTOP	Stop字符
		VKILL	Kill字符
		VSTART	Start字符
		VEOF	End-of-file字符
		VSUSP	Suspend字符
		VMIN	非规范模式读取时的最小字符数
	*/
	termio_attr.c_cc[VMIN] = 1;		/* 最小字符数 */
    termio_attr.c_cc[VTIME] = 0;	/* 设置超时时间 */

	ret = ioctl(dev_fd, TCSETA, &termio_attr);
	if (ret < 0) {
		ERR("Can't ioctl TCSETA");
	}
	/*
		#define	TCIFLUSH	0
		#define	TCOFLUSH	1
		#define	TCIOFLUSH	2
	*/
	ret = ioctl(dev_fd, TCFLSH, TCIOFLUSH);
	if (ret < 0) {
		ERR("Can't ioctl TCFLSH");
	}
	return ret;
}

EXPORT int uart_send_buf(int dev_fd, unsigned char *_buf, int len)
{
	int ret = write_hardware(dev_fd, _buf, len);
	if (ret < 0) {
		ERR("Failed to write_hardware to the output!");
		return -1;
	} else {
		return 0;
	}
}

EXPORT int uart_send_char(int dev_fd, unsigned char value)
{
	return uart_send_buf(dev_fd, &value, 1);
}

EXPORT int uart_get_buf(int dev_fd, unsigned char *buf, int len)
{
	 return read_hardware(dev_fd, buf, len);
}

EXPORT int uart_get_char(int dev_fd, unsigned char *byte_data)
{
	return uart_get_buf(dev_fd, byte_data, 1);
}

EXPORT int uart_reset_port(int dev_fd)
{
    if (ioctl(dev_fd, TCSETAW, &g_oldtermio) < 0) 
        return -1;
    else
		return 0;
}

