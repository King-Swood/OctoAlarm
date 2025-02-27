#include "HAL_Sim.h"
#include "app_hal.h"
#include "lvgl.h"

#include "demos/lv_demos.h"

extern void setup();
extern void loop();

std::optional<std::thread> toneThread;

std::array outputStates = {false};
static_assert(outputStates.size() == DigitalOutputSize);

std::array inputStates = {false};
static_assert(inputStates.size() == DigitalInputSize);

std::array analogOutputStates = {uint8_t(0)};
static_assert(analogOutputStates.size() == AnalogOutputSize);

void HALDigitalWrite(eDigitalOutput output, bool value)
{
    outputStates[static_cast<int>(output)] = value;
}

bool HALDigitalWriteReadState(eDigitalOutput output)
{
    return outputStates[static_cast<int>(output)];
}

bool HALDigitalRead(eDigitalInput input)
{
    return inputStates[static_cast<int>(input)];
}

void HALDigitalReadWriteState(eDigitalInput input, bool value)
{
    inputStates[static_cast<int>(input)] = value;
}

void HALAnalogWrite(eAnalogOutput output, uint8_t value)
{
    analogOutputStates[static_cast<int>(output)] = value;
}

uint8_t HALAnalogWriteRead(eAnalogOutput output)
{
    return analogOutputStates[static_cast<int>(output)];
}

void HALToneStop()
{
    if (toneThread) {
        auto result = system("pkill speaker-test");
        toneThread->detach();
        toneThread.reset();
    }
}

void HALToneStart(unsigned long frequency)
{
    static std::string str;

    HALToneStop();
    str = "speaker-test -t sine -f " + std::to_string(frequency) +
          " -l 0 >/dev/null 2>&1";
    toneThread.emplace(&system, str.c_str());
}

static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_PRESSED) {
        HALDigitalReadWriteState(eDigitalInput::AlarmButton, false);
    }
    else if (code == LV_EVENT_RELEASED) {
        HALDigitalReadWriteState(eDigitalInput::AlarmButton, true);
    }
}

static lv_obj_t *cbHeartbeat;
static lv_obj_t *btnAlarmButtonLED;

/**
 * Create a button with a label and react on click event.
 */
void CreateGUI()
{
    lv_obj_set_flex_flow(lv_screen_active(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_screen_active(), LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    cbHeartbeat = lv_checkbox_create(lv_screen_active());
    lv_checkbox_set_text(cbHeartbeat, "Hearbeat LED");

    btnAlarmButtonLED = lv_button_create(lv_screen_active());
    lv_obj_add_event_cb(btnAlarmButtonLED, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_size(btnAlarmButtonLED, 120, 120);
}

namespace {
constexpr lv_color_t CalcAlarmButtonColour(uint8_t analogValue)
{
    // Limit the range to half brightness up to full brightness.
    analogValue /= 2;
    analogValue += std::numeric_limits<uint8_t>::max() / 2;
    return lv_color_t{analogValue, analogValue, analogValue};
}
} // namespace

extern "C" void AppLoop()
{
    loop();

    if (HALDigitalWriteReadState(eDigitalOutput::HeartbeatLED)) {

        // HALConsolePrint("Set to true\n");
        lv_obj_add_state(cbHeartbeat, LV_STATE_CHECKED);
    }
    else {
        // HALConsolePrint("Set to false\n");
        lv_obj_remove_state(cbHeartbeat, LV_STATE_CHECKED);
    }

    lv_obj_set_style_bg_color(
        btnAlarmButtonLED,
        CalcAlarmButtonColour(HALAnalogWriteRead(eAnalogOutput::AlarmLED)), 0);
}

int main(void)
{
    HALDigitalReadWriteState(eDigitalInput::AlarmButton, true);

    lv_init();

    hal_setup();
    setup();

    CreateGUI();

    hal_loop();

    HALToneStop();
}