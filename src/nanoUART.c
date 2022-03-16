///////////////////////////////////////////////////////////////////////////
//
// nanoUART.c
//
// Author: Sangeeta Bokadia
//
// Date: Sep 24 2020
//
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>

#include <sys/ioctl.h>
#include <termios.h>

#include "nanoUART.h"

int baudrateFromSpeed(speed_t speed) {
	int br;
	switch(speed) {
		case B0: br = 0; break;
		case B50: br = 50; break;
		case B75: br = 75; break;
		case B110: br = 110; break;
		case B134: br = 134; break;
		case B150: br = 150; break;
		case B200: br = 200; break;
		case B300: br = 300; break;
		case B600: br = 600; break;
		case B1200: br = 1200; break;
		case B1800: br = 1800; break;
		case B2400: br = 2400; break;
		case B4800: br = 4800; break;
		case B9600: br = 9600; break;
		case B19200: br = 19200; break;
		case B38400: br = 38400; break;
		case B57600: br = 57600; break;
		case B115200: br = 115200; break;
		case B230400: br = 230400; break;
		default: br = -1;	
	}

	return br;
}

speed_t speedFromBaudRate(int baud_rate) {
	speed_t speed;

	switch(baud_rate) {
		case 50: speed = B50; break;
		case 75: speed = B75; break;
		case 110: speed = B110; break;
		case 134: speed = B134; break;
		case 150: speed = B150; break;
		case 200: speed = B200; break;
		case 300: speed = B300; break;
		case 600: speed = B600; break;
		case 1200: speed = B1200; break;
		case 1800: speed = B1800; break;
		case 2400: speed = B2400; break;
		case 4800: speed = B4800; break;
		case 9600: speed = B9600; break;
		case 19200: speed = B19200; break;
		case 38400: speed = B38400; break;
		case 57600: speed = B57600; break;
		case 115200: speed = B115200; break;
		case 230400: speed = B230400; break;
		default: speed = B0;
	}

	return speed;
}

int openUARTDevice(char *name, int baud_rate) {
	int fd;
	char buf[15];
	struct termios ts, ts_set;
	int res;
	speed_t speed;
	int options;


	snprintf(buf, 14, "/dev/tty%s", name);

	printf("Opening %s\n", buf);

	fd = open(buf, O_RDWR | O_NDELAY | O_NOCTTY | O_NONBLOCK);

	if (fd < 0) {
		printf("Failed to open device %s\n",
				strerror(errno));
		return -1;
	}

	//fcntl(fd, F_SETFL, O_RDWR);

	cfmakeraw(&ts);

	ts.c_oflag &= ~(OPOST);
	ts.c_cflag &= ~(CSIZE | CSTOPB | PARENB);
	ts.c_cflag |= CS8;
	ts.c_cflag |= (CLOCAL | CREAD);
	ts.c_lflag &= ~(ECHO | ISIG | ECHONL | ICANON);
	ts.c_cc[VMIN] = 0;
	ts.c_cc[VTIME] = 100;

	speed = speedFromBaudRate(baud_rate);
	if (speed == B0 && baud_rate !=0) {
		printf("Invalid baud rate : %d\n", baud_rate);
		return -1;
	}	
	if (cfsetspeed(&ts, speed) < 0) {
		printf("Error in setting speed %s\n",
				strerror(errno));
		close(fd);
		return -1;
	}
	
	if (tcsetattr(fd, TCSANOW, &ts) < 0) {
		printf("Error in setting termios: %s\n",
				strerror(errno));
		close(fd);
		return -1;
	}	

	res = tcgetattr(fd, &ts_set);
	speed = cfgetispeed(&ts_set);
	if (baudrateFromSpeed(speed) != baud_rate) {
		printf("Could not set baud rate %d for the channel",
			       baud_rate);
		return -1;	
	}

	if (ioctl(fd, TIOCMGET, &options) < 0) {
		printf("Error in getting status of modem bits %s\n",
				strerror(errno));
		close(fd);
	}

	options |= TIOCM_RTS;
	options |= TIOCM_DTR;

	if (ioctl(fd, TIOCMSET, &options) < 0) {
		printf("Error in setting status of modem bits %s\n",
				strerror(errno));
		close(fd);
	}		


	return fd;
}

int flushAllUARTData(int fd) {
	int res;
	res = tcflush(fd, TCIOFLUSH);
	return res;
}


int UARTDataAvailable(int fd) {
        int len;

        if (ioctl(fd, FIONREAD, &len) < 0) {
                printf("error in getting input data length %s\n",
                                strerror(errno));
                return 0;
        }

	return len;
}


int readDataUART(int fd, char *data, int length) {	

	int res;

	res = read(fd, data, length);

	if (res < 0) {
		printf("Error in reading data: %s\n",
				strerror(errno));
	}

	return res;
}

int writeDataUART(int fd, char *data, int length) {
	int res;

	res = write(fd, data, length);
	if (res < 0) {
                printf("Error in writing data: %s\n",
                                strerror(errno));
        }

        return res;
}


void closeUARTDevice(int fd) {
	close(fd);
}

