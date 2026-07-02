#pragma once

#include <Arduino.h>

class SolarLogic {
public:
    static void update();

    static String getStateString();

private:
    static String state;
};