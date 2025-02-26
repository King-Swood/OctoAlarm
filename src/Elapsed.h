#pragma once
#include "HAL.h"

class tElapsedMS {
  public:
    bool HasElapsed(long unsigned periodMS) const
    {
        return (millis() - originalMS_) >= periodMS;
    }
    bool HasElapsedRestart(long unsigned periodMS)
    {
        if (HasElapsed(periodMS)) {
            Restart();
            return true;
        }
        return false;
    }
    void Restart() { originalMS_ = millis(); }

  private:
    long unsigned originalMS_{millis()};
};

class tElapsedUS {
  public:
    bool HasElapsed(long unsigned periodUS) const
    {
        return (micros() - originalUS_) >= periodUS;
    }
    bool HasElapsedRestart(long unsigned periodUS)
    {
        if (HasElapsed(periodUS)) {
            Restart();
            return true;
        }
        return false;
    }
    void Restart() { originalUS_ = micros(); }

  private:
    long unsigned originalUS_{micros()};
};