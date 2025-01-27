#include <Arduino.h>
#include <EEPROM.h>

#include <config.h>

const uint8_t cmdSize = 8;

const char writeCommand[cmdSize] = "FD:CFGW";
const char readCommand[cmdSize] = "FD:CFGR";

char inputBuffer[128];

Config config;

const unsigned long millisBetweenSerialChecks = 50;
unsigned long lastSerialCheck = millis();

void saveToEeprom()
{
    EEPROM.put(0, config);
}

void initConfig()
{
    EEPROM.get(0, config);

    if (config.gain > 100 || config.gain < 0)
    {
        config.gain = defaultGain;
        saveToEeprom();
    }
}

Config getConfig()
{
    return config;
}

boolean isCommand(const char *command)
{
    for (uint8_t i = 0; i < cmdSize - 1; i++)
    {
        if (inputBuffer[i] != command[i])
        {
            return false;
        }
    }

    return true;
}

void trimInputBuffer()
{
    for (uint8_t i = 0; i < cmdSize; i++)
    {
        inputBuffer[i] = '0';
    }
}

void printCurrentConfig()
{
    Serial.print("FD:CFG:");
    Serial.print(config.gain);
}

bool processSerialCfgCmds()
{
    if (lastSerialCheck - millis() < millisBetweenSerialChecks)
    {
        return false;
    }

    auto configChange = false;
    lastSerialCheck = millis();
    if (Serial.available() > 0)
    {
        Serial.readBytes(inputBuffer, Serial.available());

        if (isCommand(readCommand))
        {
            printCurrentConfig();
        }
        else if (isCommand(writeCommand))
        {
            trimInputBuffer();
            uint8_t newGain = atoi(inputBuffer);

            config.gain = newGain;
            saveToEeprom();

            printCurrentConfig();

            configChange = true;
        }
    }

    return configChange;
}