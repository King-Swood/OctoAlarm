#include "Button.h"
#include "Globals.h"
#include "HAL.h"
#include "Heartbeat.h"
#include "LEDPulse.h"
#include "Radio.h"

class tAppRemote {
  public:
    tAppRemote() { ledPulse.SetConstantBrightness(50); }

    void Update()
    {
        heartbeat.Update();
        ledPulse.Update();
        button.Update();

        bool firstTime = state_ != lastState_;
        lastState_ = state_;

        switch (state_) {
        case eState::Idle:
            if (firstTime) {
                ledPulse.Stop();
            }
            if (button.JustReleased()) {
                state_ = eState::Flashing;
                radio.SendCommand();
            }
            break;
        case eState::Flashing:
            if (firstTime) {
                ledPulse.Start();
            }
            if (ledPulse.IsPausedOff()) {
                state_ = eState::Idle;
            }
            break;
        case eState::Size:
            break;
        }
    }

  private:
    static constexpr long unsigned ButtonHoldMS = 1000;
    tHeartbeat<eDigitalOutput::HeartbeatLED, 1000> heartbeat{};
    tLEDPulse<eAnalogOutput::AlarmLED, 2000> ledPulse{};
    tButton<eDigitalInput::AlarmButton> button{};
    tRadioTX radio{};

    enum class eState { Idle, Flashing, Size };
    eState state_{};
    eState lastState_{eState::Size};
    bool releasedInThemeState_{};
};
