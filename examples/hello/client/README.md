
**NOTES**:

* Do not try to debug using Serial, as it is used for XBee
  communications. Use SoftwareSerial instead.

* Restore defaults of every device:
    XCTU: R. Click -> Show defaults; Write

* Make sure that API level is correct

    * 1 for PC devices
    * 2 for Arduino devices

* Check that both XBee devices are using the same channel. To ensure
  that:

    * set Channel Verification to ENABLED, JV=1. Save changes.
    * change pan id to anything and restart node
    * read params, when channel is 0, set pan id to the correct one. Save changes.
    * read params, when channel is correct, set JV=0 again.

* Check devices are connected using xbee-discover

* Make changes permanent with "xbee-config -s"

* Make sure that this channel is not used by other XBee devices
