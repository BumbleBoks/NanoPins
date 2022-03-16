///////////////////////////////////////////////////////////////////////////
//
// nanoPWM.h
//
// Author: Sangeeta Bokadia
//
// Date: Sep 29 2020
//
// Pulse width modulation functions
//
///////////////////////////////////////////////////////////////////////////

#ifndef NANOPWM
#define NANOPWM

#define MAX_BUF_SIZE 64

// opens pwm on a given chip
int nano_PWM_export(unsigned int chip, unsigned int pwm);

// enables pwm on given chip
int nano_PWM_enable(unsigned int chip, unsigned int pwm);

// sets period of pulse
int nano_PWM_set_period(unsigned int chip, unsigned int pwm,
		unsigned int period_ns);

// sets duty cycle of pulse
int nano_PWM_set_dutycycle(unsigned int chip, unsigned int pwm,
                unsigned int dutycycle_ns);

// disbales pwm on given chip
int nano_PWM_disable(unsigned int chip, unsigned int pwm);

// closes pwm on a given chip
int nano_PWM_unexport(unsigned int chip, unsigned int pwm);

int nano_PWM_export_enable(unsigned int chip, unsigned int pwm);

int nano_PWM_disable_unexport(unsigned int chip, unsigned int pwm);

#endif
