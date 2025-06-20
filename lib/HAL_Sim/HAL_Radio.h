#pragma once
#include "HALTypes.h"
#include <stdlib.h>

class tHALRadioBase {
  public:
    bool IsOpen() const { return open_; }

  protected:
    bool open_{};
};

class tHALRadioRX : public tHALRadioBase {
  public:
    void Update() {}

    bool DataReceived() const { return false; }

    tRadioPacket GetPacket() const { return tRadioPacket(); }
};

class tHALRadioTX : public tHALRadioBase {
  public:
    bool SendPacket(const tRadioPacket &packet) { return false; }
};

/*

Demonstrates simple RX and TX operation.
Any of the Basic_TX examples can be used as a transmitter.
Please read through 'NRFLite.h' for a description of all the methods available
in the library.


*/

// By default, 'init' configures the radio to use a 2MBPS bitrate on channel 100
// (channels 0-125 are valid). Both the RX and TX radios must have the same
// bitrate and channel to communicate with each other. You can run the
// 'ChannelScanner' example to help select the best channel for your
// environment. You can assign a different bitrate and channel as shown below.
//   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE2MBPS,
//   100) // THE DEFAULT _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN,
//   NRFLite::BITRATE1MBPS, 75) _radio.init(RADIO_ID, PIN_RADIO_CE,
//   PIN_RADIO_CSN, NRFLite::BITRATE250KBPS, 0)
