#pragma once

class Globals {
  public:
    static constexpr auto Version = "0.010";
};

#define ArrayItemCount(a, item) (sizeof(a) / sizeof(item))