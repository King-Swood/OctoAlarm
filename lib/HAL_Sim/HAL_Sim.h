#pragma once
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

enum ePinMode { OUTPUT, INPUT, INPUT_PULLUP };
static inline void pinMode(unsigned pin, ePinMode mode)
{
    // TODO: Finish me...
}

static inline bool digitalRead(unsigned pin)
{
    // TODO: Finish me...
    return false;
}

static inline void digitalWrite(unsigned pin, bool value)
{
    // TODO: Finish me...
}

static inline void analogWrite(unsigned pin, unsigned value)
{
    // TODO: Finish me...
}

static inline void noTone(unsigned pin)
{
    // TODO: Finish me...
}

static inline void ConsoleInit() {}

static inline void ConsolePrint(const char *str)
{
    std::cout << str << std::flush;
}

static inline void tone(unsigned /*pin*/, unsigned long freq)
{
    static std::optional<std::thread> toneThread;
    static std::string str;
    if (toneThread) {
        system("pkill speaker-test");
        toneThread->detach();
    }

    str = "speaker-test -t sine -f " + std::to_string(freq) + " -l 0";
    toneThread.emplace(&system, str.c_str());
}