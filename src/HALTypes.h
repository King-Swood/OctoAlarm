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

struct tRadioPacket {
    static constexpr auto MaxSize = 32U;
    uint8_t data[MaxSize]{};
    uint8_t dataLength{};
};