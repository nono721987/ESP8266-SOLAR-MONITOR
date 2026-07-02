#pragma once

#include <Arduino.h>

struct ConfigData {
    String wifi_ssid;
    String wifi_pass;
    String mqtt_server;
    int mqtt_port;
};

class ConfigManager {
public:
    static void begin();
    static void load();
    static void save();

    static ConfigData& get();

private:
    static ConfigData config;
};