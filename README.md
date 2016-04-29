Description
===========

This is an endpoint for IceC using a XBee transceiver (using the API
mode Level 2, not the AT mode). An stringfied proxy should look like
the following:

    Light -e 1.0 -d:xbee -p panid -n nodeid

Debian installation
===================

You may install using the following command:

    $ sudo apt-get install icec-xbee

Then, you need to add a link on your Arduino libraries path:

    $ ln -s /usr/share/arduino/libraries/XBeeEndpoint/ $HOME/Arduino/libraries
