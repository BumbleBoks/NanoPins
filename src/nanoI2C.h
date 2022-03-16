///////////////////////////////////////////////////////////////////////////
//
// nanoI2C.h
//
// Author: Sangeeta Bokadia
//
// Date: Sep 19 2020
//
// Functions for I2C data transfer
//
///////////////////////////////////////////////////////////////////////////

#ifndef NANOI2C
#define NANOI2C

typedef struct {
	int  fd;
	int addr;
}i2c_device_info;

// opens i2c device
int openI2CDevice(int adapter_nr);

// reads data received from i2c device
int readI2CData(i2c_device_info device, uint8_t reg, unsigned int length, unsigned char *data);

// sends data to i2c device
int writeI2CData(i2c_device_info device, uint8_t reg, unsigned int length, unsigned char *data);

// closes i2c device 
void closeI2CDevice(int fd);

#endif

