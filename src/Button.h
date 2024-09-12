#pragma once
#include <Arduino.h>

class tButton {
public:
  tButton(int pin) : pin_(pin) { pinMode(pin_, INPUT_PULLUP); }
  bool IsPressed() const { return pressed_; }
  bool IsReleased() const { return !pressed_; }
  bool JustChanged() const { return pressed_ != lastPressed_; }
  bool JustPressed() const { return JustChanged() && IsPressed(); }
  bool JustReleased() const { return JustChanged() && IsReleased(); }
  void Update()
  {
    lastPressed_ = pressed_;
    pressed_ = !digitalRead(pin_);
  }

private:
  int pin_{};
  bool pressed_{};
  bool lastPressed_{};
};