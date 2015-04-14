#!/usr/bin/python
# -*- mode: python; coding: utf-8 -*-

import sys
import Ice

Ice.loadSlice("hello.ice")
import Example


class HelloI(Example.Hello):
    def sayHello(self, name, current):
        print " - Hello,", name


class Server(Ice.Application):
    def run(self, args):
        ic = self.communicator()

        # xbee endpoint: -p {PAN_ID} -n {NODE_NAME}
        endps = "xbee -p mypan -n node1"
        oa = ic.createObjectAdapterWithEndpoints("adapter", endps)
        oa.activate()

        servant = HelloI()
        oid = ic.stringToIdentity("server")
        print "Proxy @ '{}'".format(oa.add(servant, oid).ice_datagram())

        self.shutdownOnInterrupt()
        ic.waitForShutdown()


if __name__ == "__main__":
    Server().main(sys.argv, "server.config")
