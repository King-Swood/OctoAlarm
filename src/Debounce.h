#pragma once
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
      if ((millis() - lastTimeMS_) >= DebounceTimeMS) {
        state_ = newState;
        justChanged_ = true;
      }
    }
    else {
      lastTimeMS_ = millis();
    }
  }

private:
  T state_{};
  long unsigned lastTimeMS_{};
  bool justChanged_{};
};