CC = gcc
CFLAGS = -fPIC

OBJECTS = debug/nanoGPIO.o \
	debug/nanoI2C.o \
	debug/nanoUART.o \
	debug/nanoPins.o \
	debug/nanoPWM.o

LIBFLAGS = -li2c
TARGET = libnanopins.so

$(TARGET) : $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(TARGET) -o $(TARGET) $(OBJECTS) $(LIBFLAGS)

debug/%.o : src/%.c src/%.h
	$(CC) $(CFLAGS) -o $@ -c $<


.PHONY : clean
clean :
	-rm $(TARGET) $(OBJECTS)


depend:
	makedepend -- $(CFLAGS) -- $(FILES)

