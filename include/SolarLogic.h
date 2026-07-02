#pragma once

#include <Arduino.h>

enum SolarState {
    OFF,
    IDLE,
    ACTIVE,
    ERROR
};

class SolarLogic {
public:
    static void update();

    static SolarState getState();
    static String getStateString();

private:
    static SolarState state;
};