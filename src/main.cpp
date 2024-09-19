#include "Button.h"
#include "Globals.h"
#include "Heartbeat.h"
#include "LEDPulse.h"
#include "PatternProcessor.h"
#include "Siren.h"
#include <Arduino.h>

namespace {
constexpr int HeartbeatLEDPin = 13;
constexpr int ButtonLEDPin = 10;
constexpr int ButtonPin = 11;
constexpr int BeeperPin = 3;

tHeartbeat<HeartbeatLEDPin, 1000> heartbeat{};
tLEDPulse<ButtonLEDPin, 2000> ledPulse{};
tButton<ButtonPin> button{};
tSiren<BeeperPin, 3000> siren{};

#define ArrayItemCount(a, item) (sizeof(a) / sizeof(item))

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

bool running{};
bool lastRunning{};

void loop()
{
  heartbeat.Update();
  ledPulse.Update();
  button.Update();
  siren.Update();

  if (button.JustPressed()) {
    if (!running) {
      running = true;
      ledPulse.Start();
      siren.Start();
    }
    else {
      running = false;
      ledPulse.Stop();
      siren.Stop();
    }
  }
}