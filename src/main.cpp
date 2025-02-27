#include "Button.h"
#include "Globals.h"
#include "HAL.h"
#include "Heartbeat.h"
#include "LEDPulse.h"
#include "PatternProcessor.h"
#include "Siren.h"
#include "Theme.h"

namespace {
constexpr long unsigned ButtonHoldMS = 1000;

tHeartbeat<eDigitalOutput::HeartbeatLED, 1000> heartbeat{};
tLEDPulse<eAnalogOutput::AlarmLED, 2000> ledPulse{};
tButton<eDigitalInput::AlarmButton> button{};
#if !DISABLE_SIREN
tSiren<3000> siren{};
#endif
tThemePlayer themePlayer{};

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

    ledPulse.SetConstantBrightness(50);
}

enum class eState { Idle, Alarming, Theme, Size };

eState state_{};
eState lastState_{eState::Size};
bool releasedInThemeState_{};

void loop()
{
    heartbeat.Update();
    ledPulse.Update();
    button.Update();
#if !DISABLE_SIREN
    siren.Update();
#endif

    bool firstTime = state_ != lastState_;
    lastState_ = state_;

    switch (state_) {
    case eState::Idle:
        if (firstTime) {
            ledPulse.Stop();
#if !DISABLE_SIREN
            siren.Stop();
#endif
        }
        if (button.JustReleased()) {
            state_ = eState::Alarming;
        }
        else if (button.IsHeld(ButtonHoldMS)) {
            state_ = eState::Theme;
        }
        break;
    case eState::Alarming:
        if (firstTime) {
            ledPulse.Start();
#if !DISABLE_SIREN
            siren.Start();
#endif
        }
        if (button.JustReleased()) {
            state_ = eState::Idle;
        }
        break;
    case eState::Theme:
        if (firstTime) {
            releasedInThemeState_ = false;
            themePlayer = {};
        }

        themePlayer.Update();

        if (themePlayer.IsFinished()) {
            state_ = eState::Idle;
        }
        else if (button.JustReleased()) {
            if (!releasedInThemeState_) {
                releasedInThemeState_ = true;
            }
            else {
                state_ = eState::Idle;
            }
        }
        break;
    case eState::Size:
        break;
    }
}