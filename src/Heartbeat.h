#pragma once
#include "Elapsed.h"
#include "HAL.h"

template <eDigitalOutput Output, long unsigned Period> class tHeartbeat {
  public:
    tHeartbeat() { HALDigitalWrite(Output, false); }

    void Update()
    {
        if (lastTimeMS_.HasElapsedRestart(Period)) {
            state_ = !state_;
            HALDigitalWrite(Output, state_);
        }
    }

  private:
    bool state_{};
    tElapsedMS lastTimeMS_{};
};