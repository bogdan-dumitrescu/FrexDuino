#include <Arduino.h>
#include <EEPROM.h>

#include <config.h>
#include <constants.h>

const uint8_t cmdSize = 8;
const char initialized[7] = "ISINIT";

const uint8_t configAddress = sizeof(initialized);

const char writeCommand[cmdSize] = "FD:CFGW";
const char readCommand[cmdSize] = "FD:CFGR";
const char replyCommand[cmdSize] = "FD:CFG#";
const char paramSeparator[2] = "#";

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
        config.gainPercentage = DEFAULT_GAIN;
        config.rotationDegrees = DEFAULT_ROTATION;

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

void printCurrentConfig()
{
    Serial.print(replyCommand);
    Serial.print(config.gainPercentage);
    Serial.print(paramSeparator);
    Serial.print(config.rotationDegrees);
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
            byte index = 0;
            char *strings[16];
            char *ptr = NULL;

            // "FD:CFGW#25#360#" (save 25% gain and 360 degrees of rotation)
            ptr = strtok(inputBuffer, paramSeparator);

            while (ptr != NULL)
            {
                strings[index] = ptr;
                index++;
                ptr = strtok(NULL, paramSeparator);
            }

            uint8_t newGain = atoi(strings[1]);
            uint16_t newRotation = atoi(strings[2]);

            if (newGain >= MIN_GAIN && newGain <= MAX_GAIN)
            {
                config.gainPercentage = newGain;
                configChange = true;
            }

            if (newRotation >= MIN_ROTATION && newRotation <= MAX_ROTATION)
            {
                config.rotationDegrees = newRotation;
                configChange = true;
            }

            saveToEeprom();

            printCurrentConfig();
        }
    }

    return configChange;
}
