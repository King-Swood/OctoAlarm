#pragma once
// TODO: Remove unused includes.
#include "HALTypes.h"
#include <array>
#include <cassert>
#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <linux/kd.h>
#include <optional>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <thread>

using tTimeUnsigned = uint64_t;

namespace Private {
static inline std::optional<std::thread> toneThread;

static inline std::array outputStates{false};
static_assert(outputStates.size() == DigitalOutputSize);

static inline std::array inputStates{false};
static_assert(inputStates.size() == DigitalInputSize);

static inline std::array analogOutputStates{uint8_t(0)};
static_assert(analogOutputStates.size() == AnalogOutputSize);
} // namespace Private

static inline tTimeUnsigned millis()
{
    using namespace std::chrono;
    return std::chrono::duration<double, std::milli>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
}

static inline tTimeUnsigned micros()
{
    using namespace std::chrono;
    return std::chrono::duration<double, std::micro>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
}

static inline void HALDigitalWrite(eDigitalOutput output, bool value)
{
    Private::outputStates[static_cast<int>(output)] = value;
}

static inline bool HALDigitalRead(eDigitalInput input)
{
    return Private::inputStates[static_cast<int>(input)];
}

static inline void HALAnalogWrite(eAnalogOutput output, uint8_t value)
{
    Private::analogOutputStates[static_cast<int>(output)] = value;
}

static inline void HALConsolePrint(const char *str)
{
    std::cout << str << std::flush;
}

static inline void HALToneStop()
{
    if (Private::toneThread) {
        auto result = system("pkill speaker-test");
        Private::toneThread->detach();
        Private::toneThread.reset();
    }
}

static inline void HALToneStart(unsigned long frequency)
{
    static std::string str;

    HALToneStop();
    str = "speaker-test -t sine -f " + std::to_string(frequency) +
          " -l 0 >> NULL";
    Private::toneThread.emplace(&system, str.c_str());
}