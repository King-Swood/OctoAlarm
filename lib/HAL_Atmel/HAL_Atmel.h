#pragma once
#include "Globals.h"
#include "HALTypes.h"
#include <Arduino.h>

using tTimeUnsigned = unsigned long;

namespace Private {

static const int AnalogOutputPins[] = {
    8 // AlarmLED
};
static_assert(ArrayItemCount(AnalogOutputPins, int) == AnalogOutputSize);

struct tInput {
    int pin;
    enum class ePullup { No, Yes };
    ePullup pullup{ePullup::No};
};
static const tInput DigitalInputPins[] = {
    {7, tInput::ePullup::Yes} // AlarmButton
};
static_assert(ArrayItemCount(DigitalInputPins, tInput) == DigitalInputSize);

// Not connected, as the onboard LED shares a pin with SCLK.
static const int DigitalOutputPins[] = {
    6 // HeartbeatLED
};
static_assert(ArrayItemCount(DigitalOutputPins, int) == DigitalOutputSize);

static constexpr int BeeperPin = 3;
} // namespace Private

static inline void HALInit()
{
    for (auto i = 0U; i < DigitalOutputSize; i++) {
        pinMode(Private::DigitalOutputPins[i], OUTPUT);
        // If any pins need to be set to something other than low,
        // we will need to create an "InitialState" table.
        digitalWrite(Private::DigitalOutputPins[i], false);
    }

    for (auto i = 0U; i < DigitalInputSize; i++) {
        const auto &pinInfo = Private::DigitalInputPins[i];
        if (pinInfo.pullup == Private::tInput::ePullup::Yes) {
            pinMode(pinInfo.pin, INPUT_PULLUP);
        }
        else {
            pinMode(pinInfo.pin, INPUT);
        }
    }

    for (auto i = 0U; i < AnalogOutputSize; i++) {
        pinMode(Private::AnalogOutputPins[i], OUTPUT);
        // If any pins need to be set to something other than 0,
        // we will need to create an "InitialState" table.
        analogWrite(Private::AnalogOutputPins[i], 0);
    }

    pinMode(Private::BeeperPin, OUTPUT);
    Serial.begin(115200);
}

static inline tTimeUnsigned HALMillis() { return millis(); }

static inline tTimeUnsigned HALMicros() { return micros(); }

static inline void HALConsolePrint(const char *str) { Serial.print(str); }

static inline void HALDigitalWrite(eDigitalOutput output, bool value)
{
    digitalWrite(Private::DigitalOutputPins[static_cast<int>(output)], value);
}

static inline bool HALDigitalRead(eDigitalInput input)
{
    return digitalRead(
               Private::DigitalInputPins[static_cast<int>(input)].pin) == HIGH;
}

static inline void HALAnalogWrite(eAnalogOutput output, uint8_t value)
{
    analogWrite(Private::AnalogOutputPins[static_cast<int>(output)], value);
}

static inline void HALToneStart(unsigned long frequency)
{
    tone(Private::BeeperPin, frequency);
}

static inline void HALToneStop() { noTone(Private::BeeperPin); }