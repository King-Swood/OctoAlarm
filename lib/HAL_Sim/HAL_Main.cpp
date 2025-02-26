#include "HAL_Sim.h"

extern void setup();
extern void loop();

int main()
{
    setup();

    while (true) {
        loop();
    }
}