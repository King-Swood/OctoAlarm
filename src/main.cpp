#include "Button.h"
#include "Globals.h"
#include "Heartbeat.h"
#include "LEDPulse.h"
#include "PatternProcessor.h"
#include <Arduino.h>

namespace {
constexpr unsigned int FreqStart = 300;
constexpr unsigned int FreqEnd = 600;
constexpr unsigned int PauseTimeUS = 3000;

constexpr int HeartbeatLEDPin = 13;
constexpr int ButtonLEDPin = 10;
constexpr int ButtonPin = 11;
constexpr int BeeperPin = 3;

tHeartbeat<HeartbeatLEDPin, 1000> heartbeat{};
tLEDPulse<ButtonLEDPin, 2000> ledPulse{};
tButton<ButtonPin> button{};

#define ArrayItemCount(a, item) (sizeof(a) / sizeof(item))

void StartupPatternBlocking()
{

  struct tStartupPatternValue {
    bool ledState;
    int beeperFreq;
  };
  using tStartupPatternItem = tPatternItem<tStartupPatternValue>;

  static const tStartupPatternItem StartupPattern[] = {
      {{true, 500}, 500},   {{false, 1000}, 500}, {{true, 500}, 500},
      {{false, 1000}, 500}, {{false, 0}, 0},
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

unsigned int currentFreq = FreqStart;
bool running{};
bool lastRunning{};

void loop()
{
  heartbeat.Update();
  ledPulse.Update();
  button.Update();

  if (button.JustPressed()) {
    if (!running) {
      running = true;
      ledPulse.Start();
    }
    else {
      running = false;
      ledPulse.Stop();
    }
  }
  //   lastRunning = running;
  //   button.Update();
  //   if (button.JustPressed()) {
  //     running = !running;
  //   }

  //   if (lastRunning != running) {
  //     if (running) {
  //       currentFreq = FreqStart;
  //     }
  //   }

  //   if (running) {
  //     if (currentFreq > FreqEnd) {
  //       currentFreq = FreqStart;
  //     }
  //     tone(BeeperPin, currentFreq);
  //     currentFreq++;
  //     delayMicroseconds(PauseTimeUS);
  //   }
  //   else {
  //     noTone(BeeperPin);
  //   }
}