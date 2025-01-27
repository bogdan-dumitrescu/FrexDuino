#include <Arduino.h>

const uint8_t defaultGain = 25;

struct Config
{
    uint8_t gain = defaultGain;
};

void initConfig();
Config getConfig();
bool processSerialCfgCmds();
