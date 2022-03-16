///////////////////////////////////////////////////////////////////////////
//
// nanoGPIO.h
//
// Author: Sangeeta Bokadia
//
// Date: Aug 25 2020
//
// Functions to setup and use GPIO ports
//
///////////////////////////////////////////////////////////////////////////

#ifndef NANOGPIO
#define NANOGPIO

#define MAX_BUF_SIZE 64

typedef enum {
	GPIO_LOW = 0,
	GPIO_HIGH = 1,
	GPIO_LEVEL_SIZE =2
} gpio_level;

typedef enum {
	GPIO_IN = 3,
	GPIO_OUT = 4
} gpio_dir;

int nano_GPIO_export(unsigned int pin);

int nano_GPIO_dir(unsigned int pin, gpio_dir dir);

int nano_GPIO_write_value(unsigned int pin, gpio_level value);

int nano_GPIO_read_value(unsigned int pin);

int nano_GPIO_unexport(unsigned int pin);

#endif
