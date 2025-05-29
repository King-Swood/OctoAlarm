#pragma once
#include "HALTypes.h"
#include "NRFLite.h"
#include "SPI.h"
#include <Arduino.h>

/*

Radio    Arduino
CE    -> 9
CSN   -> 10 (Hardware SPI SS)
MOSI  -> 11 (Hardware SPI MOSI)
MISO  -> 12 (Hardware SPI MISO)
SCK   -> 13 (Hardware SPI SCK)
IRQ   -> No connection
VCC   -> No more than 3.6 volts
GND   -> GND

*/

class tHALRadioBase {
  public:
    tHALRadioBase()
    {
        if (radio_.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
            open_ = true;
        }
    }

    bool IsOpen() const { return open_; }

  protected:
    // Our radio's id.
    const static uint8_t RADIO_ID = 87;
    const static uint8_t PIN_RADIO_CE = 9;
    const static uint8_t PIN_RADIO_CSN = 10;

    NRFLite radio_;
    bool open_{};
};

class tHALRadioRX : public tHALRadioBase {
  public:
    tHALRadioRX() { radio_.startRx(); }

    void Update()
    {
        if (!IsOpen() || dataReceived_) {
            return;
        }

        auto dataLength = radio_.hasData();

        if (dataLength > 0) {
            radio_.readData(packet_.data);
            packet_.dataLength = dataLength;
            dataReceived_ = true;
        }
    }

    bool DataReceived() const { return dataReceived_; }

    tRadioPacket GetPacket() const
    {
        dataReceived_ = false;
        return packet_;
    }

  private:
    tRadioPacket packet_{};
    mutable bool dataReceived_{};
};

class tHALRadioTX : public tHALRadioBase {
  public:
    tHALRadioTX() = default;

    bool SendPacket(const tRadioPacket &packet)
    {
        return radio_.send(
                   RADIO_ID,
                   const_cast<void *>(static_cast<const void *>(packet.data)),
                   packet.dataLength, NRFLite::REQUIRE_ACK) != 0;
    }
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
