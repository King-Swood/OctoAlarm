#include "AppAlarm.h"

namespace {
tAppAlarm appAlarm{};

void StartupPatternBlocking()
{
    struct tStartupPatternValue {
        bool ledState;
        int beeperFreq;
    };
    using tStartupPatternItem = tPatternItem<tStartupPatternValue>;

    static const tStartupPatternItem StartupPattern[] = {
        {{true, 1000}, 50},
        {{false, 0}, 50},
        {{true, 1000}, 50},
        {{false, 0}, 500},
    };

    tPatternProcessor<tStartupPatternItem> startupProcessor(
        StartupPattern, ArrayItemCount(StartupPattern, tStartupPatternItem));

    while (!startupProcessor.IsFinished()) {
        startupProcessor.Update([](const tStartupPatternValue &value) {
            HALDigitalWrite(eDigitalOutput::HeartbeatLED, value.ledState);
            if (value.beeperFreq == 0) {
                HALToneStop();
            }
            else {
                HALToneStart(value.beeperFreq);
            }
        });
    }
}
} // namespace

void setup()
{
    HALInit();
    HALConsolePrint("OctoAlarm Version ");
    HALConsolePrint(Globals::Version);
    HALConsolePrint("\n");

    StartupPatternBlocking();
}

void loop() { appAlarm.Update(); }