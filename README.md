Data transfer via header pins on Jetson Nano

Notes:
- libi2c-dev must be installed on the system to build this library
  (It is used for i2c functions)

- To build the library
	make -f Makefile 

- To use it
  gcc ..
	-I /path_to_library/src
        -L /path_to_library
	-R /path_to_library
        -lananopins


