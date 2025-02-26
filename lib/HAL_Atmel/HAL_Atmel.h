#pragma once
#include <Arduino.h>

static inline void ConsoleInit() { Serial.begin(115200); }

static inline void ConsolePrint(const char *str) { Serial.print(str); }