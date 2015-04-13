# -*- mode: makefile-gmake; coding: utf-8 -*-

all:

install:
	install -d $(DESTDIR)/usr/src/IceC
	install -m 644 IceC/*.c $(DESTDIR)/usr/src/IceC

	install -d $(DESTDIR)/usr/include/IceC
	install -m 644 IceC/*.h $(DESTDIR)/usr/include/IceC

.PHONY: clean
clean:
	make -C example $@
	find -name "*.o" -delete
	find -name "*~" -delete
