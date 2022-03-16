///////////////////////////////////////////////////////////////////////////
//
// nanoPins.c
//
// Author: Sangeeta Bokadia
//
// Date: Sep 2 2020
//
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include "nanoGPIO.h"
#include "nanoPins.h"

int read_from_pin(unsigned int pin) {
	int s, val;

	s = nano_GPIO_export(pin);
	if (s == 0) {
		printf("error in GPIO export\n");
		return -1;
	}

	nano_GPIO_dir(pin, GPIO_IN);
	val = nano_GPIO_read_value(pin);
        nano_GPIO_unexport(pin);

	return val;
}

void write_to_pin(unsigned int pin, int val) {
	int s;

	s = nano_GPIO_export(pin);
	if (s == 0) {
		printf("error in GPIO export\n");
		return;
	}

	nano_GPIO_dir(pin, GPIO_OUT);
	nano_GPIO_write_value(pin, val);
        nano_GPIO_unexport(pin);

	return;
}

