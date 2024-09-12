#include <Arduino.h>
#include "Button.h"

const int ButtonPin = 2;

const int LEDPin = 13;
const int BeeperPin = 9;
const unsigned int FreqStart = 300;
const unsigned int FreqEnd = 600;
const unsigned int PauseTimeUS = 3000;

void setup()
{
    Serial.begin(9600);
    pinMode(LEDPin, OUTPUT);
}

unsigned int currentFreq = FreqStart;
tButton button{ButtonPin};
bool running{};
bool lastRunning{};

void loop()
{
    lastRunning = running;
    button.Update();
    if (button.JustPressed())
    {
        running = !running;
    }

    if (lastRunning != running)
    {
        if (running)
        {
            currentFreq = FreqStart;
        }
    }

    if (running)
    {
        if (currentFreq > FreqEnd)
        {
            currentFreq = FreqStart;
        }
        tone(BeeperPin, currentFreq);
        currentFreq++;
        delayMicroseconds(PauseTimeUS);
    }
    else
    {
        noTone(BeeperPin);
    }

    // digitalWrite(LEDPin, HIGH);
    // tone(BeeperPin, 440);
    // //   delayMicroseconds(m_pause);
    // delay(250);
    // digitalWrite(LEDPin, LOW);
    // noTone(BeeperPin);
    // delay(250);
}