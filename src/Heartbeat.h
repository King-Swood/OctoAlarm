#pragma once
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
    if ((millis() - lastTimeMS_) >= Period) {
      lastTimeMS_ = millis();
      state_ = !state_;
      digitalWrite(PIN, state_);
    }
  }

private:
  bool state_{};
  long unsigned lastTimeMS_{millis()};
};