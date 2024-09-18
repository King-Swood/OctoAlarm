#pragma once
#include <Arduino.h>

template <typename T> struct tPatternItem {
  T value;
  long unsigned timeMS;
};

template <typename TPatternItem> class tPatternProcessor {
public:
  tPatternProcessor(const TPatternItem *data, unsigned dataSize)
      : data_(data), dataSize_(dataSize)
  {
  }

  bool IsFinished() const { return finished_; }

  template <typename FOnNewState> void Update(FOnNewState onNewState)
  {
    if (finished_) {
      return;
    }

    const auto &currentItem = data_[index_];

    if (!firstStateComplete_) {
      firstStateComplete_ = true;
      onNewState(currentItem.value);
      lastTimeMS_ = millis();
    }

    if ((millis() - lastTimeMS_) >= currentItem.timeMS) {
      lastTimeMS_ = millis();
      if (++index_ >= dataSize_) {
        finished_ = true;
      }
      else {
        onNewState(currentItem.value);
      }
    }
  }

private:
  const TPatternItem *data_{};
  unsigned dataSize_{};
  long unsigned lastTimeMS_{millis()};
  unsigned index_{};
  bool firstStateComplete_{};
  bool finished_{};
};