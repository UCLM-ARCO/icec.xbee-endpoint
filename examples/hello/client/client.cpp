// -*- mode: c++; coding: utf-8 -*-

#include <Arduino.h>
#include <XBeeEndpoint.h>

#include "hello.h"

Ice_Communicator ic;
Ice_ObjectPrx remote;


void setup() {
    // init Ice and XBee endpoint
    Ice_initialize(&ic);
    IceC_XBeeEndpoint_init(&ic);

    // create a proxy to remote object
    Ice_Communicator_stringToProxy
	(&ic, "server -d:xbee -p pan -n node", &remote);
}

void loop() {
    Ice_String name;
    Ice_String_init(name, "Jonh Doe");

    // say "hello" and wait
    Example_Hello_sayHello(&remote, name);
    delay(2000);
}
