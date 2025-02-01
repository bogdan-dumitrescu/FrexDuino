#include <Arduino.h>

struct Config
{
    uint8_t gainPercentage;
    uint16_t rotationDegrees;
};

void initConfig();
Config getConfig();
bool processSerialCfgCmds();
