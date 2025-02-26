#pragma once
#if defined(SIMULATOR)
#include "HAL_Sim.h"
#else
#include "HAL_Atmel.h"
#endif