#include <Arduino.h>

#define encoderMinValue -1600
#define encoderMaxValue 1600

void initXAxisEncoder();
int32_t getCurrentPosition();
void resetCurrentPosition();