#pragma once
#include <Arduino.h>

template <unsigned PIN, long unsigned UpdatePeriodUS> class tLEDPulse {
public:
  tLEDPulse()
  {
    pinMode(PIN, OUTPUT);
    Update();
  }

  void Start() { start_ = true; }
  void Stop() { stop_ = true; }
  void SetConstantBrightness(unsigned pwmValue)
  {
    stop_ = true;
    constantPWMValue_ = pwmValue;
  }

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
        pwmValue_ = 0;
        analogWrite(PIN, constantPWMValue_);
      }
      break;
    case eState::Increasing:
      if (firstTime) {
        pwmValue_ = 0;
      }
      if ((micros() - lastTimeUS_) >= UpdatePeriodUS) {
        lastTimeUS_ = micros();
        if (pwmValue_ < MaxPWM) {
          pwmValue_++;
        }
        else {
          state_ = eState::PauseBright;
        }
      }
      analogWrite(PIN, pwmValue_);
      break;
    case eState::PauseBright:
      if (firstTime) {
        lastTimeUS_ = micros();
      }
      if ((micros() - lastTimeUS_) >= PauseBrightUS) {
        state_ = eState::Decreasing;
      }
      break;
    case eState::Decreasing:
      if (firstTime) {
        pwmValue_ = MaxPWM;
      }
      if ((micros() - lastTimeUS_) >= UpdatePeriodUS) {
        lastTimeUS_ = micros();
        if (pwmValue_ > 0) {
          pwmValue_--;
        }
        else {
          state_ = eState::PauseOff;
        }
      }
      analogWrite(PIN, pwmValue_);
      break;
    case eState::PauseOff:
      if (firstTime) {
        lastTimeUS_ = micros();
      }
      if ((micros() - lastTimeUS_) >= PauseOffUS) {
        state_ = eState::Increasing;
      }
      break;
    case eState::Size:
      break;
    }
  }

private:
  static constexpr auto MaxPWM = 255;
  static constexpr auto PauseBrightUS = UpdatePeriodUS * 100;
  static constexpr auto PauseOffUS = UpdatePeriodUS * 100;
  unsigned pwmValue_{};
  unsigned constantPWMValue_{};
  long unsigned lastTimeUS_{micros()};
  enum eState { Off, Increasing, PauseBright, Decreasing, PauseOff, Size };
  eState state_{};
  eState lastState_{};
  bool start_{};
  bool stop_{};
};