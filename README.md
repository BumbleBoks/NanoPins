Data transfer via header pins on Jetson Nano

Notes:
- libi2c-dev must be installed on the system to build this library
  (It is used for i2c functions)

- To build the library
	make -f Makefile 

- To use it
  gcc ...
	-I /path to library/src
        -L /path to library
        -R /path to library
        -lananopins


