#pragma once
#include <Arduino.h>

struct SensorConfig {
    String vorlaufID;
    String ruecklaufID;
    String caseID;
};

struct ConfigData {
    String wifi_ssid;
    String wifi_pass;

    String mqtt_server;
    int mqtt_port;

    SensorConfig sensors;   // 👈 DAS ist der neue Teil
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