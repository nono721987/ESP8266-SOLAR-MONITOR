#pragma once

#include <Arduino.h>

class App {
public:
    static void begin();
    static void loop();
    static void log(const String &msg);

private:
    static unsigned long lastStateUpdate;
    static unsigned long lastHistoryUpdate;
};