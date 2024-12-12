#pragma once
#include "Elapsed.h"
#include <Arduino.h>

template <typename T, long unsigned DebounceTimeMS> class tDebounce {
  public:
    tDebounce(T initialState = T()) : state_(initialState) {}

    T State() const { return state_; }

    bool JustChanged() const { return justChanged_; }

    void Update(T newState)
    {
        justChanged_ = false;

        if (newState != state_) {
            if (lastTimeMS_.HasElapsedRestart(DebounceTimeMS)) {
                state_ = newState;
                justChanged_ = true;
            }
        }
        else {
            lastTimeMS_.Restart();
        }
    }

  private:
    T state_{};
    tElapsedMS lastTimeMS_{};
    bool justChanged_{};
};