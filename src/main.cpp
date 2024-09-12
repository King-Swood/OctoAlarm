#include "Button.h"
#include "Globals.h"
#include "Heartbeat.h"
#include <Arduino.h>

const int ButtonPin = 2;

const int BeeperPin = 9;
const unsigned int FreqStart = 300;
const unsigned int FreqEnd = 600;
const unsigned int PauseTimeUS = 3000;

tHeartbeat<13, 1000> heartbeat;

void setup()
{
  Serial.begin(115200);
  Serial.print("OctoAlarm Version ");
  Serial.println(Globals::Version);
}

unsigned int currentFreq = FreqStart;
tButton button{ButtonPin};
bool running{};
bool lastRunning{};

void loop()
{
  heartbeat.Update();
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