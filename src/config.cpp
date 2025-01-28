#include <Arduino.h>
#include <EEPROM.h>

#include <config.h>
#include <constants.h>

const uint8_t cmdSize = 8;
const char initialized[7] = "ISINIT";

const uint8_t configAddress = sizeof(initialized);

const char writeCommand[cmdSize] = "FD:CFGW";
const char readCommand[cmdSize] = "FD:CFGR";

char inputBuffer[128];

Config config;

const unsigned long millisBetweenSerialChecks = 50;
unsigned long lastSerialCheck = millis();

void saveToEeprom()
{
    EEPROM.put(configAddress, config);
}

bool configInitialized()
{
    EEPROM.get(0, inputBuffer);

    for (uint8_t i = 0; i < sizeof(initialized) - 1; i++)
    {
        if (inputBuffer[i] != initialized[i])
        {
            return false;
        }
    }

    return true;
}

void initConfig()
{
    if (configInitialized())
    {
        EEPROM.get(configAddress, config);
    }
    else
    {
        config.gain = DEFAULT_GAIN;
        saveToEeprom();
        EEPROM.put(0, initialized);
    }
}

Config getConfig()
{
    return config;
}

bool isCommand(const char *command)
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