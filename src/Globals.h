#pragma once

class Globals {
  public:
    static constexpr auto Version = "1.000";
};

#define ArrayItemCount(a, item) (sizeof(a) / sizeof(item))