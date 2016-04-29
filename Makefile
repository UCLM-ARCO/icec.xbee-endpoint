# -*- mode: makefile-gmake; coding: utf-8 -*-

all:

install: LIBDIR = $(DESTDIR)/usr/share/arduino/libraries/XBeeEndpoint
install:
	install -d $(LIBDIR)
	install -m 644 arduino/* $(LIBDIR)
	install -m 644 IceC/*.cpp $(LIBDIR)
	install -m 644 IceC/*.h $(LIBDIR)

.PHONY: clean
clean:
	make -C examples $@
	find -name "*.o" -delete
	find -name "*~" -delete
