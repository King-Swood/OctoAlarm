#pragma once
#include "HAL.h"

class tElapsedMS {
  public:
    bool HasElapsed(long unsigned periodMS) const
    {
        return (HALMillis() - originalMS_) >= periodMS;
    }
    bool HasElapsedRestart(long unsigned periodMS)
    {
        if (HasElapsed(periodMS)) {
            Restart();
            return true;
        }
        return false;
    }
    void Restart() { originalMS_ = HALMillis(); }

  private:
    long unsigned originalMS_{HALMillis()};
};

class tElapsedUS {
  public:
    bool HasElapsed(long unsigned periodUS) const
    {
        return (HALMicros() - originalUS_) >= periodUS;
    }
    bool HasElapsedRestart(long unsigned periodUS)
    {
        if (HasElapsed(periodUS)) {
            Restart();
            return true;
        }
        return false;
    }
    void Restart() { originalUS_ = HALMicros(); }

  private:
    long unsigned originalUS_{HALMicros()};
};