Description
===========

This is an endpoint for IceC using a XBee transceiver (using the API
mode, not the AT mode). An stringfied proxy should look like the
following:

    Light -e 1.0 -d:xbee -p panid -n nodeid

Manual installation
===================

To manually install this endpoint, create a directory on your Arduino
*libraries* path, and make a link there to ``IceC/*.{c,cpp}`` and to
``IceC`` directory. Just as follows:

    $ mkdir ~/Arduino/libraries/XBeeEndpoint
    $ cd ~/Arduino/libraries/XBeeEndpoint
    $ ln -s /path/to/this/repo/IceC/*.c .
    $ ln -s /path/to/this/repo/IceC/*.h .
    $ ln -s /path/to/this/repo/IceC/*.cpp .
    $ ln -s /path/to/this/repo/IceC .
