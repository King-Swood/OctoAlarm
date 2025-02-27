#pragma once
#pragma once
#include "ToneGenerator.h"
#include <cassert>
#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <linux/kd.h>
#include <optional>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

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
    static std::optional<ToneGenerator> tone;
    if (!tone) {
        tone.emplace();
        tone->setAmplitude(0.5f);
    }

    tone->setFrequency(freq);
}

// static inline void tone(unsigned /*pin*/, unsigned long freq)
// {
//     static bool isOpen{};
//     static FILE *tty{};
//     if (!isOpen) {
//         if (NULL == (tty = fopen("/dev/console", "w"))) {
//             fprintf(stderr, "Cannot write to /dev/console!\n");
//             exit(1);
//         }
//     }
//     static constexpr auto ESC = 27U;
//     static constexpr auto Duration = 10000;
//     fprintf(tty, "%c[10;%d]%c[11;%d]\a", ESC, freq, ESC, Duration);
//     // static bool isOpen{};
//     // static int fd{};
//     // if (!isOpen) {
//     //     fd = open("/dev/console", O_WRONLY);
//     //     assert(fd);
//     // }
//     // static constexpr int MS = 10000;
//     // ioctl(fd, KDMKTONE, (MS << 16 | 1193180 / freq));
//     // ConsolePrint("Played tone\n");

//     // //     int fd = open("/dev/console", O_RDONLY);
//     // //  if (fd == -1 || argc != 3) return -1;
//     // //  return ioctl(fd, KDMKTONE,
//     // (atoi(argv[2])<<16)+(1193180/atoi(argv[1])));
// }