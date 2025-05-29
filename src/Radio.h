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

        tRadioPacket packet{};
        packet.data[0] = 'h';
        packet.data[1] = 'i';
        packet.data[2] = 'a' + counter_;
        if (++counter_ > ('z' - 'a')) {
            counter_ = 0;
        }
        packet.data[3] = '\0';
        packet.dataLength = 4;
        auto result = HALRadioTXInstance().SendPacket(packet);
        if (!result) {
            HALConsolePrint("Radio failed to send command\n");
        }
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
            auto packet = radio.GetPacket();
            packet.data[packet.MaxSize - 1] = 0;
            HALConsolePrint(reinterpret_cast<const char *>(packet.data));
            HALConsolePrint("\n");
        }
    }

  private:
    unsigned counter_{};
};