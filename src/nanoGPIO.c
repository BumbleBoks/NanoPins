///////////////////////////////////////////////////////////////////////////
//
// nanoGPIO.c
//
// Author: Sangeeta Bokadia
//
// Date: Aug 25 2020
//
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <string.h>
#include <errno.h>
#include "nanoGPIO.h"

int nano_GPIO_export(unsigned int pin) {
	int fd, n, res;
	char buf[MAX_BUF_SIZE];

        fd = open("/sys/class/gpio/export", O_WRONLY, 0);
 	if (fd == -1) {
		printf("Error in GPIO export!\n");
		return 0;
	}	
        n = sprintf(buf, "%d", pin);
        res = write(fd, buf, n);
        if (res < 0) {
                printf("Error in writing to GPIO export: %s\n",
                                strerror(errno));
        }

       	close(fd);
	return res;	
}

int nano_GPIO_dir(unsigned int pin, gpio_dir dir) {
        int fd, n;
	char gpio_name[50];

	sprintf(gpio_name, "/sys/class/gpio/gpio%d/direction",
			pin);

        fd = open(gpio_name, O_WRONLY, 0);
        if (fd == -1) {
                printf("Error in GPIO direction for pin %d!\n", pin);
                return 0;
        }

	if (dir == GPIO_IN) {
		write(fd, "in\0", GPIO_IN);
	} else {
		write(fd, "out\0", GPIO_OUT);
	}

        close(fd);
        return 1;
}


int nano_GPIO_write_value(unsigned int pin, gpio_level value) {
        int fd, n;
        char gpio_name[50];

        sprintf(gpio_name, "/sys/class/gpio/gpio%d/value",
                        pin);

        fd = open(gpio_name, O_WRONLY, 0);
        if (fd == -1) {
                printf("Error in writing value at GPIO pin %d!\n", pin);
                return 0;
        }

	if (value == GPIO_HIGH) {
		write(fd, "1\0", GPIO_LEVEL_SIZE);
	}
	else {
		write(fd, "0\0", GPIO_LEVEL_SIZE);
	}
        close(fd);
        return 1;
}

int nano_GPIO_read_value(unsigned int pin) {
	int fd, n;
        char gpio_name[50];
        char buf[MAX_BUF_SIZE];

        sprintf(gpio_name, "/sys/class/gpio/gpio%d/value",
                        pin);

        fd = open(gpio_name, O_RDONLY, 0);
        if (fd == -1) {
                printf("Error in reading value at GPIO pin %d!\n", pin);
                return 0;
        }

	n = read(fd, buf, GPIO_LEVEL_SIZE);

        close(fd);

	if (n > 0) {
		return atoi(buf);
	}

        return n;
}

int nano_GPIO_unexport(unsigned int pin) {
	int fd, n, res;
        char buf[MAX_BUF_SIZE];

        fd = open("/sys/class/gpio/unexport", O_WRONLY, 0);
        if (fd == -1) {
                printf("Error in GPIO unexport!\n");
                return 0;
        }

	n = sprintf(buf, "%d", pin);
        res = write(fd, buf, n);
        if (res < 0) {
                printf("Error in writing to GPIO unexport: %s\n",
                                strerror(errno));
        }

        close(fd);
        return res;
}
