#include "so_lib.h"
#include "sl_spi.h"
#include <linux/spi/spidev.h>

DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);


#define SPI_MAX_SPEED 25000000

EXPORT int set_spi_writeBits_PerWord(int spi_fd, int bits) 
{
	int ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret < 0) {
		ERR("Can't ioctl SPI_IOC_WR_BITS_PER_WORD");
	}
	return ret;
}

EXPORT int set_spi_max_speed(int spi_fd, unsigned int spi_speed) 
{
	int ret;
    if (spi_speed > SPI_MAX_SPEED) {
        ERR("invalid spi speed %d", spi_speed);
    }
	ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	if (ret < 0) {
		ERR("Can't ioctl SPI_IOC_WR_MAX_SPEED_HZ");
		return ret;
	}
	return ret;
}

EXPORT int set_spi_data_mode(int spi_fd, int mode)
{
	int ret = 0;
	int spi_mode = 0;
	switch (mode) {
		case SPI_MODE0: 
			spi_mode &= ~(SPI_CPOL | SPI_CPHA);
			break;
		case SPI_MODE1: 
			spi_mode &= ~SPI_CPOL;
			spi_mode |= SPI_CPHA;
			break;
		case SPI_MODE2:
			spi_mode |= SPI_CPOL;
			spi_mode &= ~SPI_CPHA;
			break;
		case SPI_MODE3:
			spi_mode |= SPI_CPOL | SPI_CPHA;
			break;
		default :
			ERR("%s error", __func__);
			return -1;
	}

	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode);
	if (ret < 0) {
		ERR("Can't ioctl SPI_IOC_WR_MODE");
	}
	return ret;
}

/*
	struct spi_ioc_transfer {
	__u64		tx_buf;
	__u64		rx_buf;

	__u32		len;
	__u32		speed_hz;

	__u16		delay_usecs;
	__u8		bits_per_word;
	__u8		cs_change;
	__u32		pad;

	 * If the contents of 'struct spi_ioc_transfer' ever change
	 * incompatibly, then the ioctl number (currently 0) must change;
	 * ioctls with constant size fields get a bit more in the way of
	 * error checking than ones (like this) where that field varies.
	 *
	 * NOTE: struct layout is the same in 64bit and 32bit userspace.
	 *
*/

EXPORT int write_bytes_to_spi(
		int spi_fd
		, unsigned char * writeData
		, int writeLen
		, int spi_delay
		, int spi_speed
		, int spi_bits
		) 
{
	unsigned int len = writeLen;

	unsigned char * pWriteData = writeData;
	/* 
		天嵌开发板上运行时rx和txbuf必须要有值,不能为0，不然会报错
	*/
	unsigned char rx[50];

	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)pWriteData;
	tr.rx_buf = (unsigned long)rx;
	tr.len = len;
	tr.delay_usecs = spi_delay;
	tr.speed_hz = spi_speed;
	tr.bits_per_word = spi_bits;

	int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 0) {
		ERR("Can't ioctl SPI_IOC_MESSAGE");
	}
	return ret;
}

EXPORT int read_bytes_from_spi(
	 int spi_fd
	 , unsigned char * readBuffer
	 , int readLen
	 , int spi_delay
	 , int spi_speed
	 , int spi_bits
	 ) 
{
	unsigned int len = readLen;

	unsigned char * pReadBuffer = readBuffer;
	unsigned char tx[50];

	struct spi_ioc_transfer tr;
	tr.tx_buf = (unsigned long)tx;
	tr.rx_buf = (unsigned long)pReadBuffer;
	tr.len = len;
	tr.delay_usecs = spi_delay;
	tr.speed_hz = spi_speed;
	tr.bits_per_word = spi_bits;

	int ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
    
	if (ret < 0) {
		ERR("Can't ioctl SPI_IOC_MESSAGE");
	}
	return ret;
}


