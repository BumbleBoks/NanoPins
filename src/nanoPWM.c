///////////////////////////////////////////////////////////////////////////
//
// nanoPWM.c
//
// Author: Sangeeta Bokadia
//
// Date: Sep 30 2020
//
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>

#include "nanoPWM.h"

int nano_PWM_export(unsigned int chip, unsigned int pwm) {
	int n, fd, res;
	char buf[MAX_BUF_SIZE];

	snprintf(buf, 31, "/sys/class/pwm/pwmchip%d/export", chip); 
	fd = open(buf, O_WRONLY, 0);
	if (fd < 0) {
		printf("Error in pwmchip export.\n");
		return 0;
	}

	n = sprintf(buf, "%d", pwm);
	res = write(fd, buf, n);

	if (res < 0) {
		printf("Error in writing to pwmchip export: %s\n",
				strerror(errno));
	}

	close(fd);
	return res;
}

int nano_PWM_set_period(unsigned int chip, unsigned int pwm,
                unsigned int period_ns) {
	// period in ns
	int n, fd, res;
        char buf[MAX_BUF_SIZE];

	snprintf(buf, 36, "/sys/class/pwm/pwmchip%d/pwm%d/period", 
			chip, pwm);
	fd = open(buf, O_WRONLY, 0);
	if (fd < 0) {
		printf("Error in pwm set period.\n");
                return 0;
        }

	n = sprintf(buf, "%d", period_ns);

        res = write(fd, buf, n);

        if (res < 0) {
                printf("Error in writing to pwmchip pwm period: %s\n",
                                strerror(errno));
        }

        close(fd);

	return res;
}

int nano_PWM_set_dutycycle(unsigned int chip, unsigned int pwm,
                unsigned int dutycycle_ns) {
        // period in ns
        int n, fd, res;
        char buf[MAX_BUF_SIZE];

        snprintf(buf, 40, "/sys/class/pwm/pwmchip%d/pwm%d/duty_cycle",
                        chip, pwm);
        fd = open(buf, O_WRONLY, 0);
        if (fd < 0) {
                printf("Error in pwm set duty cycle.\n");
                return 0;
        }

        n = sprintf(buf, "%d", dutycycle_ns);

        res = write(fd, buf, n);

        if (res < 0) {
                printf("Error in writing to pwmchip wpm dutycycle: %s\n",
                                strerror(errno));
        }

        close(fd);

        return res;
}

int nano_PWM_enable(unsigned int chip, unsigned int pwm) {
        int fd, res;
        char buf[MAX_BUF_SIZE];
	const int n = 1;

        snprintf(buf, 36, "/sys/class/pwm/pwmchip%d/pwm%d/enable", 
			chip, pwm);
        fd = open(buf, O_WRONLY, 0);
        if (fd < 0) {
                printf("Error in pwmchip pwm enable.\n");
                return 0;
        }

       
        res = write(fd, "1", n);
        if (res < 0) {
                printf("Error in writing to pwmchip pwm enable: %s\n",
                                strerror(errno));
        }

        close(fd);
        return res;
}

int nano_PWM_disable(unsigned int chip, unsigned int pwm) {
        int fd, res;
        char buf[MAX_BUF_SIZE];
        const int n = 1;

        snprintf(buf, 36, "/sys/class/pwm/pwmchip%d/pwm%d/enable",
                        chip, pwm);
        fd = open(buf, O_WRONLY, 0);
        if (fd < 0) {
                printf("Error in pwmchip pwm disable.\n");
                return 0;
        }


        res = write(fd, "0", n);
        if (res < 0) {
                printf("Error in writing to pwmchip pwm disable: %s\n",
                                strerror(errno));
        }

        close(fd);
        return res;
}


int nano_PWM_unexport(unsigned int chip, unsigned int pwm) {
        int n, fd, res;
        char buf[MAX_BUF_SIZE];

        snprintf(buf, 33, "/sys/class/pwm/pwmchip%d/unexport", chip);
        fd = open(buf, O_WRONLY, 0);
        if (fd < 0) {
                printf("Error in pwmchip unexport.\n");
                return 0;
        }

        n = sprintf(buf, "%d", pwm);
        res = write(fd, buf, n);

        if (res < 0) {
                printf("Error in writing to pwmchip unexport: %s\n",
                                strerror(errno));
        }

        close(fd);
        return res;
}

int nano_PWM_export_enable(unsigned int chip, unsigned int pwm) {
	int res;
	
	res = nano_PWM_export(chip, pwm);
	if (res > 0) {
		res = nano_PWM_enable(chip, pwm);
	}
	return res;
}

int nano_PWM_disable_unexport(unsigned int chip, unsigned int pwm) {
        int res;

        nano_PWM_disable(chip, pwm);
	res = nano_PWM_unexport(chip, pwm);
       
        return res;
}
