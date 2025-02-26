#pragma once
#include "Debounce.h"
#include "Elapsed.h"
#include "HAL.h"

template <int PIN> class tButton {
  public:
    tButton() { pinMode(PIN, INPUT_PULLUP); }
    bool IsPressed() const { return pressed_; }
    bool IsReleased() const { return !pressed_; }
    bool JustChanged() const { return pressed_ != lastPressed_; }
    bool JustPressed() const { return JustChanged() && IsPressed(); }
    bool JustReleased() const { return JustChanged() && IsReleased(); }
    bool IsHeld(long unsigned holdTimeMS) const
    {
        return pressed_ && pressedTimeMS_.HasElapsed(holdTimeMS);
    }
    void Update()
    {
        lastPressed_ = pressed_;
        debouncedInput_.Update(!digitalRead(PIN));
        pressed_ = debouncedInput_.State();
        if (JustPressed()) {
            pressedTimeMS_.Restart();
        }
    }

  private:
    tDebounce<bool, 50> debouncedInput_;
    tElapsedMS pressedTimeMS_{};
    bool pressed_{};
    bool lastPressed_{};
};