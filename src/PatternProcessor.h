#pragma once
#include "Elapsed.h"
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
            lastTimeMS_.Restart();
        }

        if (lastTimeMS_.HasElapsedRestart(currentItem.timeMS)) {
            if (++index_ >= dataSize_) {
                finished_ = true;
            }
            else {
                const auto &nextItem = data_[index_];
                onNewState(nextItem.value);
            }
        }
    }

  private:
    const TPatternItem *data_{};
    unsigned dataSize_{};
    tElapsedMS lastTimeMS_{};
    unsigned index_{};
    bool firstStateComplete_{};
    bool finished_{};
};