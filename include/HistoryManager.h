#pragma once

#include <Arduino.h>

#define HISTORY_SIZE 120   // erstmal 1 Stunde bei 30s Intervall

struct HistoryEntry {
    float v;
    float r;
    float c;
};

class HistoryManager {
public:
    static void begin();
    static void update(float v, float r, float c);

    static HistoryEntry get(int index);
    static int size();

private:
    static HistoryEntry buffer[HISTORY_SIZE];
    static int writeIndex;
    static int count;
};