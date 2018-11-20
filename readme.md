### ﻿一、各个目录介绍


1、so为系统层 一些文件操作之类的，公共调用的

2、sl为外设层 一些gpio操作，spi操作

3、bsp为板级支持包 一些oled之类的应用函数

4、demo为调用的bsp做应用的演示

### 二、使用


1、make后，把bsp目录下的libslbsp.so和lib目录下的libslso.so拷贝到板子上
2、安装动态库：
```
	install --mode=644 libslhl.so   /usr/local/lib
	install --mode=644 libslbsp.so  /usr/local/lib
```
3、把demo目录下编译出来的可执行文件拷贝到板子上运行
### 
三、笔记
1. SPI
2. UART
- a) uart的驱动配置的主要接口在drivers/tty/tty_ioctl.c
		- 函数：	tty_mode_ioctl()
		- tty的ioctl的cmd定义在include/uapi/asm-generic/ioctl.h
- 下面是tty_mode_ioctl()函数的部分代码：
	
```
		case TCSETAF:
			return set_termios(real_tty, p, TERMIOS_FLUSH | TERMIOS_WAIT | TERMIOS_TERMIO);
		case TCSETAW:
			return set_termios(real_tty, p, TERMIOS_WAIT | TERMIOS_TERMIO);
		case TCSETA:
			return set_termios(real_tty, p, TERMIOS_TERMIO);
```
	        
- 下面是set_termios()函数部分代码:   
```    
		if (opt & TERMIOS_TERMIO) {
			if (user_termio_to_kernel_termios(&tmp_termios,
						(struct termio __user *)arg))
				return -EFAULT;
		} else if (opt & TERMIOS_OLD) {
		if (user_termios_to_kernel_termios_1(&tmp_termios,
						(struct termios __user *)arg))
				return -EFAULT;
		} else {
		if (user_termios_to_kernel_termios(&tmp_termios,
						(struct termios2 __user *)arg))
				return -EFAULT;
		}
```    
- 下面是相关的cmd低层对应的定义：
       
```
		#define TERMIOS_FLUSH	1			/* 刷新输入输出buffer */
		#define TERMIOS_WAIT	2			/* 等待直到发送完成在设置 */
		#define TERMIOS_TERMIO	4			/* 传送termio结构到内核termios */
		#define TERMIOS_OLD	8			/* user_termios_to_kernel_termios_1*/

```
   

- b) uart的配置参数定义

		c_iflag参数表:
		键 值 说 明
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

		c_oflag参数:
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

		c_lflag参数:
		ISIG	当输入INTR、QUIT、SUSP或DSUSP时，产生相应的信号
		ICANON	使用标准输入模式
		XCASE	在ICANON和XCASE同时设置的情况下，终端只使用大写。
		ECHO	显示输入字符
		ECHOE	如果ICANON同时设置，ERASE将删除输入的字符
		ECHOK	如果ICANON同时设置，KILL将删除当前行
		ECHONL	如果ICANON同时设置，即使ECHO没有设置依然显示换行符
		ECHOPRT	如果ECHO和ICANON同时设置，将删除打印出的字符（非POSIX）
		TOSTOP	向后台输出发送SIGTTOU信号

		c_cflag参数:
		CBAUD	波特率（4+1位）（非POSIX）
		CBAUDEX	附加波特率（1位）（非POSIX）
		CSIZE	字符长度，取值范围为CS5、CS6、CS7或CS8
		CSTOPB	设置停止位
		CREAD	使用接收器
		PARENB	使用奇偶校验
		PARODD	对输入使用奇偶校验，对输出使用偶校验
		HUPCL	关闭设备时挂起
		CLOCAL	忽略调制解调器线路状态
		CRTSCTS	使用RTS/CTS流控制

		c_cc支持的控制字符:
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
