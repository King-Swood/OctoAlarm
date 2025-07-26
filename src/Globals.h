#pragma once
#include "HAL.h"

class Globals {
  public:
    static constexpr auto Version = "1.001";
};

#define ArrayItemCount(a, item) (sizeof(a) / sizeof(item))

#ifndef DISABLE_SIREN
#define DISABLE_SIREN 0
#endif