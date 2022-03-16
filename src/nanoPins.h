///////////////////////////////////////////////////////////////////////////
//
// nanoPins.h
//
// Author: Sangeeta Bokadia
//
// Date: Sep 2 2020
//
// Reads GPIO digital input and writes GPIO digital outputs
//
///////////////////////////////////////////////////////////////////////////

#ifndef NANOPINS
#define NANOPINS

// Reads digital input at pin
int read_from_pin(unsigned int pin);

// Writes digital output to pin
void write_to_pin(unsigned int pin, int val);

#endif
