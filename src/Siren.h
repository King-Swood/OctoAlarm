#pragma once
#include <Arduino.h>

template <unsigned PIN, long unsigned UpdatePeriodUS> class tSiren {
public:
  tSiren()
  {
    pinMode(PIN, OUTPUT);
    Update();
  }

  void Start() { start_ = true; }
  void Stop() { stop_ = true; }

  void Update()
  {
    if (start_) {
      start_ = false;
      state_ = eState::Increasing;
      lastState_ = eState::Size;
    }
    if (stop_) {
      stop_ = false;
      state_ = eState::Off;
      lastState_ = eState::Size;
    }

    const auto firstTime = lastState_ != state_;
    lastState_ = state_;

    switch (state_) {
    case eState::Off:
      if (firstTime) {
        currentFreq = 0;
        noTone(PIN);
      }
      break;
    case eState::Increasing:
      if (firstTime) {
        currentFreq = FreqLow;
      }
      if ((micros() - lastTimeUS_) >= UpdatePeriodUS) {
        lastTimeUS_ = micros();
        if (currentFreq < FreqHigh) {
          currentFreq++;
        }
        else {
          state_ = eState::Pause;
        }
      }
      tone(PIN, currentFreq);
      break;
    case eState::Pause:
      if (firstTime) {
        lastTimeUS_ = micros();
      }
      if ((micros() - lastTimeUS_) >= PauseTimeUS) {
        state_ = eState::Increasing;
      }
      break;
    case eState::Size:
      break;
    }
  }

private:
  static constexpr auto FreqLow = 300;
  static constexpr auto FreqHigh = 600;
  static constexpr auto PauseTimeUS = 0;

  unsigned int currentFreq = FreqLow;
  long unsigned lastTimeUS_{micros()};
  enum eState { Off, Increasing, Pause, Size };
  eState state_{};
  eState lastState_{};
  bool start_{};
  bool stop_{};
};