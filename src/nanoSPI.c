///////////////////////////////////////////////////////////////////////////
//
// nanoSPI.c
//
// Author: Sangeeta Bokadia
//
// Date: Sep 6 2020
//
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include <fcntl.h>  // open file - spi /dev/spidev
#include <unistd.h> // read, write

#include <errno.h>
#include <string.h>

#include <sys/ioctl.h> // status = ioctl(fd, SPI_IOC_MESSAGE(4), mesg);
#include <linux/types.h> 
#include <linux/spi/spidev.h> // struct spi_ioc_transfer
#include "nanoSPI.h"

spi_channel_info formSPIChannel(int bus, int channel, int bits_per_word, 
		int speed) {
	spi_channel_info ch;
	ch.bus = bus;
	ch.channel = channel;
	ch.speed = speed;
	ch.bits_per_word = bits_per_word;

	return ch;
}


// mode options are SPI_MODE_0(0), SPI_MODE1(1), SPI_MODE2(2), SPI_MODE3(3)
// lsb_first options are SPI_LSB_FIRST(1) or 0
int setupSPIDevice(spi_channel_info chinfo, int mode, int lsb_first) 
{
	int fd;
	
	char buf[20];

	int spi_mode;
	
	switch (mode) {
		case 1: spi_mode = SPI_MODE_1;
		case 2: spi_mode = SPI_MODE_2;
		case 3: spi_mode = SPI_MODE_3;
		default: spi_mode = SPI_MODE_0;
	}
	
	if (lsb_first == 1) lsb_first = SPI_LSB_FIRST;
	else lsb_first = 0;


	// opens the device at /dev/spidev(bus).(channel)
	snprintf(buf, 19, "/dev/spidev%d.%d", chinfo.bus, chinfo.channel);
	fd = open(buf, O_RDWR);

	if (fd < 0) {
		printf("Failed to open device %s\n", buf);
		return fd;
	}

	// SPI settings for the device
	// set mode 
	if (ioctl(fd, SPI_IOC_WR_MODE, &spi_mode) != 0) {
		printf("Failed to set up spi mode : %s\n",
				strerror(errno));			
	}

	// set bits_per_word 
	if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &chinfo.bits_per_word) != 0) {
		printf("Failed to set up spi bits per word : %s\n",
				strerror(errno));			
	}

	// set lsb first
	if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb_first) != 0) {
		printf("Failed to set up spi bit justification : %s\n",
				strerror(errno));			
	}

 	// set data transfer speed 	
	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &chinfo.speed) != 0) {
		printf("Failed to set up spi speed : %s\n",
				strerror(errno));			
	}

	return fd;

}

// nwords - number of spi transfers
// len - length of data in spi transfer
int transferSPIData(spi_channel_info channel, unsigned char *data, size_t len, 
		int nwords) 
{
	int status;
	struct spi_ioc_transfer spi_data;
	
	spi_data.tx_buf = (unsigned long)data;
	spi_data.rx_buf = (unsigned long)data;

	spi_data.len = len;
	spi_data.speed_hz = channel.speed;

	spi_data.delay_usecs = 0;
	spi_data.bits_per_word = channel.bits_per_word;

	spi_data.cs_change = 1;
	spi_data.tx_nbits = 0;
	spi_data.rx_nbits = 0;
        spi_data.pad = 0;
		
	status = ioctl(channel.fd, SPI_IOC_MESSAGE(nwords), &spi_data);
	
	if (status < 0){
		printf("Failed to transmit: %s\n", strerror(errno));
	}	
	return status;
}

void stopSPIDevice(int fd) {
	close(fd);
}

