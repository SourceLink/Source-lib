#include "sl_i2c.h"
#include "so_lib.h"


DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);


EXPORT int set_i2c_slave(int dev_fd, int slave)
{
	int ret = ioctl(dev_fd, I2C_SLAVE, slave);
	if ( ret < 0) {
		ERR("Can't ioctl I2C_SLAVE");
	}
	return ret;
}

EXPORT int set_i2c_timeout(int dev_fd, int timeout)
{
	int ret = ioctl(dev_fd, I2C_TIMEOUT, timeout);
	if ( ret < 0) {
		ERR("Can't ioctl I2C_TIMEOUT");
	}
	return ret;
}

EXPORT int set_i2c_retries(int dev_fd, int retries)
{
	int ret = ioctl(dev_fd, I2C_RETRIES, retries);
	if ( ret < 0) {
		ERR("Can't ioctl I2C_RETRIES");
	}
	return ret;
}

static int i2c_smbus_access(int dev_fd, char read_write, unsigned char command, 
                                     int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;

	int ret = ioctl(dev_fd, I2C_SMBUS, &args);
	return ret;
}

EXPORT int i2c_write_byte(int dev_fd, unsigned char command)
{
	return i2c_smbus_access(dev_fd,I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_BYTE,NULL);
}


EXPORT int i2c_read_byte_data(int dev_fd, unsigned char command)
{
	union i2c_smbus_data data;
	if (i2c_smbus_access(dev_fd,I2C_SMBUS_READ,command,
	                     I2C_SMBUS_BYTE_DATA,&data))
		return -1;
	else
		return 0x0FF & data.byte;
}

EXPORT int i2c_write_byte_data(int dev_fd, unsigned char command, 
                                              unsigned char value)
{
	union i2c_smbus_data data;
	data.byte = value;
	return i2c_smbus_access(dev_fd,I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_BYTE_DATA, &data);
}


EXPORT int i2c_read_word_data(int dev_fd, unsigned char command)
{
	union i2c_smbus_data data;
	if (i2c_smbus_access(dev_fd,I2C_SMBUS_READ,command,
	                     I2C_SMBUS_WORD_DATA,&data))
		return -1;
	else
		return 0x0FFFF & data.word;
}

EXPORT int i2c_write_word_data(int dev_fd, unsigned char command, 
                                              unsigned short value)
{
	union i2c_smbus_data data;
	data.word = value;
	return i2c_smbus_access(dev_fd,I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_WORD_DATA, &data);
}

/*
	写进去又读出来
*/
EXPORT int i2c_process_call(int dev_fd, unsigned char command, unsigned short value)
{
	union i2c_smbus_data data;
	data.word = value;
	if (i2c_smbus_access(dev_fd,I2C_SMBUS_WRITE,command,
	                     I2C_SMBUS_PROC_CALL,&data))
		return -1;
	else
		return 0x0FFFF & data.word;
}

/* Returns the number of read bytes */
EXPORT int i2c_read_block_data(int dev_fd, unsigned char command, unsigned char *values)
{
	union i2c_smbus_data data;
	int i;
	if (i2c_smbus_access(dev_fd,I2C_SMBUS_READ,command,
	                     I2C_SMBUS_BLOCK_DATA,&data))
		return -1;
	else {
		for (i = 1; i <= data.block[0]; i++)
			values[i-1] = data.block[i];
		return data.block[0];	/* block[0] is used for length */
	}
}

EXPORT int i2c_write_block_data(int dev_fd, unsigned char command, 
                                               unsigned char length, unsigned char *values)
{
	union i2c_smbus_data data;
	int i;
	if (length > 32)
		length = 32;
	for (i = 1; i <= length; i++)
		data.block[i] = values[i-1];
	data.block[0] = length;
	return i2c_smbus_access(dev_fd,I2C_SMBUS_WRITE,command,
	                        I2C_SMBUS_BLOCK_DATA, &data);
}

