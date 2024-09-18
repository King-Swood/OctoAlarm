#include "Button.h"
#include "Globals.h"
#include "Heartbeat.h"
#include "LEDPulse.h"
#include "PatternProcessor.h"
#include <Arduino.h>

const unsigned int FreqStart = 300;
const unsigned int FreqEnd = 600;
const unsigned int PauseTimeUS = 3000;

tHeartbeat<13, 1000> heartbeat{};
tLEDPulse<10, 2000> ledPulse{};
tButton<11> button{};

// TODO: On startup, flash the main button LED and beep twice, along with the
// heartbeat LED. Then set the Button LED to 50% brightness, and flash the
// heartbeat consistently.

// Buzzer is dio 3.

struct tStartupPattern {
  bool value;
  long unsigned timeMS;
};

const tStartupPattern StartupPattern[] = {
    {true, 500},
    {false, 500},
    {true, 500},
    {false, 500},
};

#define ArrayItemCount(a, item) (sizeof(a) / sizeof(item))

void setup()
{
  Serial.begin(115200);
  Serial.print("OctoAlarm Version ");
  Serial.println(Globals::Version);

  tPatternProcessor<tStartupPattern> startupProcessor(
      StartupPattern, ArrayItemCount(StartupPattern, tStartupPattern));

  while (!startupProcessor.IsFinished()) {
    startupProcessor.Update([](bool value) { digitalWrite(10, value); });
  }

  ledPulse.SetConstantBrightness(50);

  //   pinMode(3, OUTPUT);
  //   tone(3, 1000);
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