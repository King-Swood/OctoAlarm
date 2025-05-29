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
    void Update()
    {
        const auto &radio = HALRadioRXInstance();
        if (radio.IsOpen() && radio.DataReceived()) {
            const auto packet = radio.GetPacket();
            HALConsolePrint(packet.String());
            HALConsolePrint("\n");
        }
    }

  private:
    unsigned counter_{};
};