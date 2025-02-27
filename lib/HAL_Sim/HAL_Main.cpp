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

void HALAnalogWrite(eAnalogOutput output, uint8_t value)
{
    analogOutputStates[static_cast<int>(output)] = value;
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
          " -l 0 >> NULL";
    toneThread.emplace(&system, str.c_str());
}

static void btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = reinterpret_cast<lv_obj_t *>(lv_event_get_target(e));
    if (code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its
         * text*/
        lv_obj_t *label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

static lv_obj_t *cbHeartbeat;

/**
 * Create a button with a label and react on click event.
 */
void lv_example_get_started_2(void)
{
    lv_obj_set_flex_flow(lv_screen_active(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_screen_active(), LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    cbHeartbeat = lv_checkbox_create(lv_screen_active());
    lv_checkbox_set_text(cbHeartbeat, "Hearbeat LED");
    // lv_obj_add_event_cb(cb, event_handler, LV_EVENT_ALL, NULL);

    // lv_obj_t *btn = lv_button_create(
    //     lv_screen_active());       /*Add a button the current screen*/
    // lv_obj_set_pos(btn, 10, 10);   /*Set its position*/
    // lv_obj_set_size(btn, 120, 50); /*Set its size*/
    // lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL,
    //                     NULL); /*Assign a callback to the button*/

    // lv_obj_t *label = lv_label_create(btn); /*Add a label to the button*/
    // lv_label_set_text(label, "Button");     /*Set the labels text*/
    // lv_obj_center(label);
}

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
}

int main(void)
{
    lv_init();

    hal_setup();
    setup();

    // lv_demo_widgets();
    lv_example_get_started_2();

    hal_loop();
}