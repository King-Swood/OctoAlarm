#pragma once
#include "Debounce.h"
#include <Arduino.h>

template <int PIN> class tButton {
public:
  tButton() { pinMode(PIN, INPUT_PULLUP); }
  bool IsPressed() const { return pressed_; }
  bool IsReleased() const { return !pressed_; }
  bool JustChanged() const { return pressed_ != lastPressed_; }
  bool JustPressed() const { return JustChanged() && IsPressed(); }
  bool JustReleased() const { return JustChanged() && IsReleased(); }
  void Update()
  {
    lastPressed_ = pressed_;
    debouncedInput_.Update(!digitalRead(PIN));
    pressed_ = debouncedInput_.State();
  }

private:
  tDebounce<bool, 50> debouncedInput_;
  bool pressed_{};
  bool lastPressed_{};
};