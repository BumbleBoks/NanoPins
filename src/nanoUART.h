///////////////////////////////////////////////////////////////////////////
//
// nanoUART.h
//
// Author: Sangeeta Bokadia
//
// Date: Sep 23 2020
//
// UART functions
//
///////////////////////////////////////////////////////////////////////////

#ifndef NANOUART
#define NANOUART

#define min_fn(x, y) \
        ({ __typeof__ (x) _x = (x); \
         __typeof__ (y) _y = (y); \
         _x < _y ? _x : _y; })


// opens a UART device and sets baud rate
int openUARTDevice(char *name, int baud_rate);

// flushes all the data 
int flushAllUARTData(int fd);

// checks if data is available
int UARTDataAvailable(int fd);

// receives data from UART device
int readDataUART(int fd, char *data, int length);

// transmits data to UART device
int writeDataUART(int fd, char *data, int length);

// closes UART device
void closeUARTDevice(int fd);


#endif
