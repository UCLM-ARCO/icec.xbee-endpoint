# -*- mode: makefile-gmake; coding: utf-8 -*-

CXXFLAGS  += -I.
LDFLAGS   += -lIce -lIceUtil -lpthread
LDLIBS     = /usr/lib/IcePy.so.35 -lpython2.7

TARGET     = server
SLICE      = hello.ice


all: $(TARGET)

$(TARGET): $(SLICE:%.ice=%.o) server.o
	g++ $(LDFLAGS) $(LDLIBS) -o $@ $^

%.cpp: %.ice
	slice2cpp $^

.PHONY: clean
clean:
	$(RM) -r $(SLICE:%.ice=%.h) $(SLICE:%.ice=%.cpp) server *.o
