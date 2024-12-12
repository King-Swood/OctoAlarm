#include "Button.h"
#include "Globals.h"
#include "Heartbeat.h"
#include "LEDPulse.h"
#include "PatternProcessor.h"
#include "Siren.h"
#include "Theme.h"
#include <Arduino.h>

namespace {
constexpr int HeartbeatLEDPin = 13;
constexpr int ButtonLEDPin = 10;
constexpr int ButtonPin = 11;
constexpr int BeeperPin = 3;
constexpr long unsigned ButtonHoldMS = 1000;

tHeartbeat<HeartbeatLEDPin, 1000> heartbeat{};
tLEDPulse<ButtonLEDPin, 2000> ledPulse{};
tButton<ButtonPin> button{};
tSiren<BeeperPin, 3000> siren{};
tThemePlayer<BeeperPin> themePlayer{};

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
            digitalWrite(ButtonLEDPin, value.ledState);
            if (value.beeperFreq == 0) {
                noTone(BeeperPin);
            }
            else {
                tone(BeeperPin, value.beeperFreq);
            }
        });
    }
}
} // namespace

void setup()
{
    Serial.begin(115200);
    Serial.print("OctoAlarm Version ");
    Serial.println(Globals::Version);

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
    siren.Update();

    bool firstTime = state_ != lastState_;
    lastState_ = state_;

    switch (state_) {
    case eState::Idle:
        if (firstTime) {
            ledPulse.Stop();
            siren.Stop();
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
            siren.Start();
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