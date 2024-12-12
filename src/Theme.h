#pragma once
#include "PatternProcessor.h"
#include <Arduino.h>
#define TONE_USE_INT
#define TONE_PITCH 440
#include "Globals.h"
#include <TonePitch.h>

template <unsigned BeeperPin> class tThemePlayer {
  private:
    using tMelodyNote = tPatternItem<unsigned int>;

    static constexpr tMelodyNote MelodyPattern[] = {
        {NOTE_C4, 500}, {0, 500},       {NOTE_D4, 500},
        {0, 500},       {NOTE_E4, 500}, {0, 500},
    };

    tPatternProcessor<tMelodyNote> melodyProcessor{
        MelodyPattern, ArrayItemCount(MelodyPattern, tMelodyNote)};

  public:
    void Update()
    {
        melodyProcessor.Update([](unsigned value) {
            if (value == 0) {
                noTone(BeeperPin);
            }
            else {
                tone(BeeperPin, value);
            }
        });
    }

    bool IsFinished() const { return melodyProcessor.IsFinished(); }
};