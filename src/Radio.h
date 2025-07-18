#include "HAL.h"

class tRadioTX {
  public:
    bool SendCommand()
    {
        auto &radio = HALRadioTXInstance();
        if (!radio.IsOpen()) {
            HALConsolePrint("Radio didn't initialise properly\n");
            return false;
        }

        auto result =
            HALRadioTXInstance().SendPacket(tRadioPacket{"ring doorbell"});
        if (!result) {
            HALConsolePrint(
                "Radio failed to send command or didn't receive ACK\n");
        }
        return result;
    }

  private:
    unsigned counter_{};
};

class tRadioRX {
  public:
    bool CommandReceived() const { return commandReceived_; }
    void Update()
    {
        const auto &radio = HALRadioRXInstance();
        commandReceived_ = false;

        if (!firstPassComplete_) {
            if (!radio.IsOpen()) {
                HALConsolePrint("Radio didn't initialise properly\n");
            }
            firstPassComplete_ = true;
        }

        if (radio.IsOpen() && radio.DataReceived()) {
            const auto packet = radio.GetPacket();
            // TODO: Need to actually parse the packet to make sure the command
            // is correct.
            HALConsolePrint(packet.String());
            HALConsolePrint("\n");
            commandReceived_ = true;
        }
    }

  private:
    bool firstPassComplete_{};
    bool commandReceived_{};
};