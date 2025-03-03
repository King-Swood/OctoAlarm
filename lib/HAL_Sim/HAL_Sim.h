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

#define DISABLE_SIREN 1

using tTimeUnsigned = uint64_t;

static inline void HALConsolePrint(const char *str)
{
    std::cout << str << std::flush;
}

static inline tTimeUnsigned HALMillis()
{
    using namespace std::chrono;
    return std::chrono::duration<double, std::milli>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
}

static inline tTimeUnsigned HALMicros()
{
    using namespace std::chrono;
    return std::chrono::duration<double, std::micro>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
}

void HALDigitalWrite(eDigitalOutput output, bool value);
bool HALDigitalRead(eDigitalInput input);
void HALAnalogWrite(eAnalogOutput output, uint8_t value);
void HALToneStop();
void HALToneStart(unsigned long frequency);