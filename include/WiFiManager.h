#pragma once

#include <Arduino.h>

class WiFiManager {
public:
    static void begin();
    static void loop();

    static bool isConnected();
    static String getIP();

private:
    static void startAP();
    static void connectWiFi();

    static bool apMode;
};