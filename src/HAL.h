#pragma once
#if defined(SIMULATOR)
#include "HAL_Sim.h"
#else
#include "HAL_Atmel.h"
#endif

void HALInit();
void HALConsolePrint(const char *str);
tTimeUnsigned HALMillis();
tTimeUnsigned HALMicros();
void HALDigitalWrite(eDigitalOutput output, bool value);
bool HALDigitalRead(eDigitalInput input);
void HALAnalogWrite(eAnalogOutput output, uint8_t value);
void HALToneStop();
void HALToneStart(unsigned long frequency);