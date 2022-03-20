///////////////////////////////////////////////////////////////////////////
//
// nanoSPI.h
//
// Author: Sangeeta Bokadia
//
// Date: Sep 4 2020
//
///////////////////////////////////////////////////////////////////////////

#ifndef NANOSPI
#define NANOSPI

typedef struct {
	int fd;
	int bus;
	int channel;
	int speed;
	int bits_per_word;
}spi_channel_info;

spi_channel_info formSPIChannel(int bus, int channel, int bits_per_word, 
		int speed);
int setupSPIDevice(spi_channel_info chinfo, int mode, int lsb_first);
int transferSPIData(spi_channel_info channel, unsigned char *data, size_t len, int nwords);

void stopSPIDevice(int fd);

#endif
