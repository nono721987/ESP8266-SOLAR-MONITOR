#pragma once

#include <Arduino.h>

class PumpController {
public:
    static void begin();
    static void update(bool shouldRun);

    static bool isRunning();

private:
    static bool running;
};