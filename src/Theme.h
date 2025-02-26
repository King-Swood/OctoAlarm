#pragma once
#include "HAL.h"
#include "PatternProcessor.h"
#define TONE_USE_INT
#define TONE_PITCH 440
#include "Globals.h"
#include <TonePitch.h>

template <unsigned BeeperPin> class tThemePlayer {
  private:
    using tMelodyNote = tPatternItem<unsigned int>;

    static constexpr auto NoteBreak = tMelodyNote{0, 50};
    static constexpr long unsigned Quaver = 350;
    static constexpr long unsigned SemiQuaver = 150;

    static constexpr tMelodyNote MelodyPattern[] = {{NOTE_E5, Quaver},
                                                    NoteBreak,
                                                    {NOTE_E5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_E5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_G5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_E5, SemiQuaver},
                                                    NoteBreak,
                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_B4, SemiQuaver},
                                                    NoteBreak,

                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_B4, SemiQuaver},
                                                    NoteBreak,
                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_D5, SemiQuaver},
                                                    NoteBreak,
                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_B4, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_D5, Quaver},
                                                    NoteBreak,

                                                    {NOTE_E5, Quaver},
                                                    NoteBreak,
                                                    {NOTE_E5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_E5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_G5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_E5, SemiQuaver},
                                                    NoteBreak,
                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_A5, SemiQuaver},
                                                    NoteBreak,

                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_AS5, SemiQuaver},
                                                    NoteBreak,
                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_B5, SemiQuaver},
                                                    NoteBreak,
                                                    {0, Quaver},
                                                    NoteBreak,
                                                    {0, Quaver},
                                                    NoteBreak,

                                                    {NOTE_A5, Quaver},
                                                    NoteBreak,
                                                    {NOTE_A5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_A5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_C6, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_A5, SemiQuaver},
                                                    NoteBreak,
                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_E5, SemiQuaver},
                                                    NoteBreak,

                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_E5, SemiQuaver},
                                                    NoteBreak,
                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_G5, SemiQuaver},
                                                    NoteBreak,
                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_E5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_G5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_GS5, SemiQuaver},
                                                    NoteBreak,

                                                    {NOTE_A5, Quaver},
                                                    NoteBreak,
                                                    {NOTE_A5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_A5, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_C6, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_A5, SemiQuaver},
                                                    NoteBreak,
                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_D6, SemiQuaver},
                                                    NoteBreak,

                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_DS6, SemiQuaver},
                                                    NoteBreak,
                                                    {0, SemiQuaver},
                                                    NoteBreak,
                                                    {NOTE_E6, SemiQuaver},
                                                    NoteBreak,
                                                    {0, Quaver},
                                                    NoteBreak,
                                                    {0, Quaver},
                                                    NoteBreak,

                                                    {0, 1}};

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