// -*- mode: c++; coding: utf-8 -*-

#include <Ice/Ice.h>
#include "hello.h"

class HelloI : virtual public Example::Hello {
public:
    void sayHello(const std::string& name, const Ice::Current& current) {
	std::cout << " - hello, " << name << std::endl;
    }
};

class Server : virtual public Ice::Application {
public:
    virtual int run(int, char**) {
	Ice::CommunicatorPtr ic = communicator();

	Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints
	    ("Adapter", "xbee -p pan -n node");
	adapter->activate();

	Ice::ObjectPrx proxy =
	    adapter->add(new HelloI(), ic->stringToIdentity("server"));
	proxy = proxy->ice_datagram();
	proxy = proxy->ice_encodingVersion(Ice::Encoding_1_0);

	std::cout << "Proxy ready at '" << proxy << "'" << std::endl;

	shutdownOnInterrupt();
	ic->waitForShutdown();
	return 0;
    }
};

int
main(int argc, char** argv) {
    Server s;
    return s.main(argc, argv, "server.config");
}
