#pragma once

#include <Arduino.h>

class PumpController
{
public:
    static void begin();
    static void update(bool requestRun);

    static bool isRunning();

private:
    static bool running;

    static unsigned long lastStart;
    static unsigned long lastStop;
};