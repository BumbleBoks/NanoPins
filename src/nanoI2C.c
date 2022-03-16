///////////////////////////////////////////////////////////////////////////
//
// nanoI2C.c
//
// Author: Sangeeta Bokadia
//
// Date: Sep 20 2020
//
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdint.h>

#include <errno.h>
#include <string.h>

#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include "nanoI2C.h"



int openI2CDevice(int adapter_nr) {
	int fd;
	char buf[32];

	printf("open /dev/i2c-%d\n", adapter_nr);
	snprintf(buf, 31, "/dev/i2c-%d", adapter_nr);
	fd = open(buf, O_RDWR);
	if (fd < 0) {
		printf("Error in opening device\n %s", 
				strerror(errno));
	}

	return fd;
}

int readDataFromI2C(i2c_device_info device, uint8_t reg, unsigned int length, 
		unsigned char* data)
{
	int retval;

	write(device.fd, &reg, 1);
	retval = read(device.fd, data, length);
	
	return retval;
}

int readI2CData(i2c_device_info device, uint8_t reg, unsigned int length, 
		unsigned char* data)
{
	// checks if SMBUS functionality can be used (data length <= 32)
	// checks if SMBUS functionality is available for the device
	// fallback is to read directly from I2C
	__s32 res;
	int retval;
	unsigned long funcs;
	int fd = device.fd;

	printf("talk to device\n");
	if (ioctl(fd, I2C_SLAVE, device.addr) !=0) {
		printf("Failed to talk to the device %s\n",
				strerror(errno));
		return -1;
	}

	if (ioctl(fd, I2C_FUNCS, &funcs) < 0) {
		printf("Failed to get a list of supported functions %s\n", 
				strerror(errno));
		return -1;
	}

	printf("read data from device\n");

	// upto 8 bits
	if (length <= 1) {
		if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA)) {
			retval = readDataFromI2C(device, reg, length, data);
			return retval;
		}

		res = i2c_smbus_read_byte_data(fd, reg);
		if (res < 0) {
			printf("Failed to read data %s\n",
				strerror(errno));
		}

		data[0] = res & 0xFF; // last 8 bits
		return 8;
	}

	// 9-16 bits
	if (length <= 2) {
		if (!(funcs & I2C_FUNC_SMBUS_READ_WORD_DATA)) {
			retval = readDataFromI2C(device, reg, length, data);
			return retval;
		}

		res = i2c_smbus_read_word_data(fd, reg);
		if (res < 0) {
			printf("Failed to read data %s\n",
				strerror(errno));
		}

		data[1] = (res >> 8) & 0xFF; // 9-16 bits
		data[0] = res & 0xFF; // last 8 bits
		return 16;
	}

	// 17-32 bits
	if (length <= 4) {
		if (!(funcs & I2C_FUNC_SMBUS_READ_BLOCK_DATA)) {
			printf("fallback\n");
			retval = readDataFromI2C(device, reg, length, data);
			return retval;
		}

		printf("smbus\n");
		res = i2c_smbus_read_block_data(fd, reg, data);
		if (res < 0) {
			printf("Failed to read data %s\n",
				strerror(errno));
		}

		return res;
	}

	// everything else
	retval = readDataFromI2C(device, reg, length, data);
	return retval;
}

int writeDataToI2C(i2c_device_info device, uint8_t reg, unsigned int length,
                unsigned char* data) {
	
	int retval, i;
	char buf[length+1];

	buf[0] = reg;

	for (i = 0; i < length; i++) {
		buf[i+1] = data[i];
	}
	
	retval = write(device.fd, buf, length + 1);

	if (retval != (length + 1)) {
		printf("Error in writing data %s\n", strerror(errno));
	}

	return retval;
}

int writeI2CData(i2c_device_info device, uint8_t reg, unsigned int length, unsigned char *data) {
        // checks if SMBUS functionality can be used (data length <= 32)
        // checks if SMBUS functionality is available for the device
        // fallback is to write directly to I2C
        __s32 res;
	__u8 val8;
	__u16 val16;
        int retval;
        unsigned long funcs;
        int fd = device.fd;

        printf("talk to device\n");
        if (ioctl(fd, I2C_SLAVE, device.addr) !=0) {
                printf("Failed to talk to the device %s\n",
                                strerror(errno));
                return -1;
        }

        if (ioctl(fd, I2C_FUNCS, &funcs) < 0) {
                printf("Failed to get a list of supported functions %s\n",
                                strerror(errno));
                return -1;
        }

        printf("read data from device\n");

        // upto 8 bits
        if (length <= 1) {
                if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE_DATA)) {
                        retval = writeDataToI2C(device, reg, length, data);
                        return retval;
                }

		val8 = data[0];
                res = i2c_smbus_write_byte_data(fd, reg, val8);
                if (res < length) {
                        printf("Failed to write data %s\n",
                                strerror(errno));
                }

                return res;
        }

        // 9-16 bits
        if (length <= 2) {
                if (!(funcs & I2C_FUNC_SMBUS_WRITE_WORD_DATA)) {
                        retval = writeDataToI2C(device, reg, length, data);
                        return retval;
                }

		val16 = data[1];
		val16 = (val16 << 8) | data[0];

                res = i2c_smbus_write_word_data(fd, reg, val16);
                if (res < length) {
                        printf("Failed to write data %s\n",
                                strerror(errno));
                }

                return res;
        }

	// 17-32 bits
        if (length <= 4) {
                if (!(funcs & I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)) {
                        printf("fallback\n");
                        retval = writeDataToI2C(device, reg, length, data);
                        return retval;
                }

                printf("smbus\n");
                res = i2c_smbus_write_block_data(fd, reg, length, data);
                if (res < 0) {
                        printf("Failed to read data %s\n",
                                strerror(errno));
                }

                return res;
        }

        // everything else
        retval = writeDataToI2C(device, reg, length, data);
        return retval;
}


void closeI2CDevice(int fd) {
	close(fd);
}
                               
