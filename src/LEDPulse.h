#pragma once
#include <Arduino.h>

template <unsigned PIN, long unsigned UpdatePeriodUS> class tLEDPulse {
public:
  tLEDPulse()
  {
    pinMode(PIN, OUTPUT);
    Update();
  }

  void Update()
  {
    if ((micros() - lastTimeUS_) >= UpdatePeriodUS) {
      lastTimeUS_ = micros();
      if (direction_) {
        if (pwmValue_ < MaxPWM) {
          pwmValue_++;
        }
        else {
          direction_ = false;
        }
      }
      else {
        if (pwmValue_ > 0) {
          pwmValue_--;
        }
        else {
          direction_ = true;
        }
      }
      analogWrite(PIN, pwmValue_);
    }
  }

private:
  static constexpr auto MaxPWM = 255;
  unsigned pwmValue_{};
  long unsigned lastTimeUS_{micros()};
  bool direction_{};
};