#include "HAL.h"

static inline const char *PacketString = "ring doorbell";

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
            HALRadioTXInstance().SendPacket(tRadioPacket{PacketString});
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
        auto &radio = HALRadioRXInstance();
        commandReceived_ = false;

        if (!firstPassComplete_) {
            if (!radio.IsOpen()) {
                HALConsolePrint("Radio didn't initialise properly\n");
                return;
            }
            firstPassComplete_ = true;
        }

        radio.Update();

        if (radio.IsOpen() && radio.DataReceived()) {
            const auto packet = radio.GetPacket();
            if (strncmp(packet.String(), PacketString, strlen(PacketString)) ==
                0) {
                commandReceived_ = true;
            }
            else {
                HALConsolePrint("Unknown command received\n");
            }
        }
    }

  private:
    bool firstPassComplete_{};
    bool commandReceived_{};
};