#pragma once

#include <Arduino.h>

class MqttManager {
public:
    static void begin();
    static void loop();
    static void publish();

    static bool isConnected();

private:
    static void connect();
};