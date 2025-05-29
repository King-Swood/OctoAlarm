#pragma once
#include <stdint.h>

enum class eDigitalOutput { HeartbeatLED, Size };
static constexpr auto DigitalOutputSize =
    static_cast<unsigned>(eDigitalOutput::Size);

enum class eDigitalInput { AlarmButton, Size };
static constexpr auto DigitalInputSize =
    static_cast<unsigned>(eDigitalInput::Size);

enum class eAnalogOutput { AlarmLED, Size };
static constexpr auto AnalogOutputSize =
    static_cast<unsigned>(eAnalogOutput::Size);

class tRadioPacket {
  public:
    static constexpr auto MaxSize = 32U;

    tRadioPacket() = default;
    tRadioPacket(const char *str) { AddString(str); }
    void AddString(const char *str)
    {
        while (*str != '\0') {
            if (dataLength_ >= (MaxSize - 1)) {
                break;
            }
            AddChar(*str++);
        }
        AddChar('\0');
    }
    const char *String() const { return data_; }

  private:
    friend class tHALRadioRX;
    friend class tHALRadioTX;
    // These are private, as we want to limit the packet to string data.
    void AddChar(char data)
    {
        if (dataLength_ < MaxSize) {
            data_[dataLength_++] = data;
        }
    }

    char data_[MaxSize]{};
    uint8_t dataLength_{};
};