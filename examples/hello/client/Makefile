# -*- mode: makefile-gmake; coding: utf-8 -*-

BOARD_TAG     = fio
ARDUINO_LIBS  = IceC XBeeEndpoint EasyXBee XBee
MONITOR_PORT  = /dev/ttyUSB0

# NOTE: this is only needed for debug
ARDUINO_LIBS += SoftwareSerial

USER_LIB_PATH = $(HOME)/Arduino/libraries

# CFLAGS       += -DDEBUG_LED=13
CFLAGS       += -Wno-unused-function -Wno-unused-variable
CFLAGS       += -Wno-unused-but-set-variable -Wno-strict-aliasing
CXXFLAGS      = $(CFLAGS)
SLICE         = hello.ice

vpath %.ice ..

all: $(SLICE:%.ice=%.h)

%.h: %.ice
	slice2c $^

.PHONY: vclean
vclean: clean
	$(RM) -f $(SLICE:%.ice=%.h) *.o

include /usr/share/arduino/Arduino.mk
