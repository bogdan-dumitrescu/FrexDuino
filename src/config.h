#include <Arduino.h>

struct Config
{
    uint8_t gain;
};

void initConfig();
Config getConfig();
bool processSerialCfgCmds();
