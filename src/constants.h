#include <Arduino.h>

const bool DEBUG = false;

const uint8_t DEFAULT_GAIN = 25;

const uint16_t ENCODER_PPR = 1200;        // encoders pulses per revolution. Usually calculated as 4*CPR (counts per rotation).
const uint16_t WHEEL_TOTAL_DEGREES = 880; // total wheel rotation in degrees

const float WHEEL_ROTATIONS = (float)WHEEL_TOTAL_DEGREES / 360;

const float ENCODER_TOTAL_PULSES = WHEEL_ROTATIONS * ENCODER_PPR; // the Frex wheel turns once for every encoder revolution

const int16_t ENCODER_MIN_VALUE = (int16_t)(-ENCODER_TOTAL_PULSES / 2); // minimum value that the encoder will read (when wheel is turned all the way left)
const int16_t ENCODER_MAX_VALUE = (int16_t)(ENCODER_TOTAL_PULSES / 2);  // maximum value that the encoder will read (when wheel is turned all the way right)

const uint8_t CENTERING_BTN = 0;            // centering button index
const uint16_t CTR_LNG_PRS_TRESH_MS = 2500; // long press threshold (how many millis it takes to register the centering button long-press)

const uint8_t BTN_MTRX_ROW_COUNT = 2; // button matrix row count
const uint8_t BTN_MTRX_CLM_COUNT = 4; // button matrix column count

const uint8_t BTN_MTRX_ROW_PINS[BTN_MTRX_ROW_COUNT] = {12, 13};     // Arduino pins for connecting rows
const uint8_t BTN_MTRX_CLM_PINS[BTN_MTRX_CLM_COUNT] = {4, 5, 6, 7}; // Arduino pins for connecting rows

const uint8_t BTN_COUNT = BTN_MTRX_ROW_COUNT * BTN_MTRX_CLM_COUNT; // maximum number of buttons supported
