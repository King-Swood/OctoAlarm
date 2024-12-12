#pragma once
#include "Elapsed.h"
#include <Arduino.h>

template <unsigned PIN, long unsigned Period> class tHeartbeat {
  public:
    tHeartbeat()
    {
        pinMode(PIN, OUTPUT);
        digitalWrite(PIN, false);
    }

    void Update()
    {
        if (lastTimeMS_.HasElapsedRestart(Period)) {
            state_ = !state_;
            digitalWrite(PIN, state_);
        }
    }

  private:
    bool state_{};
    tElapsedMS lastTimeMS_{};
};