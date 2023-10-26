#include <Arduino.h>

#pragma once

#define VERSION_MAJOR 1
#define VERSION_MINOR 1

//#define DEBUG_DCC_PACKETS
#define DEBUG_SERVO_MOVEMENT

#define DEFAULT_ADDRESS 1

#define SERVO_COUNT 8
#define DCC_IN_PIN 2
#define PUSH_BUTTON_PIN 20
#define LED_PIN 19

#define PWM_PINS {12, 11, 13, 14, 15, 16, 17, 18}
#define POWER_PINS {10, 9, 8, 7, 6, 5, 4, 3}

#define TIMEOUT_PUSH_TO_SETUP 500L
#define TIMEOUT_PUSH_TO_FACTORY_RESET 5000L
#define TIMEOUT_SWITCH_SERVO_POS_SETUP 5000L
#define TIMEOUT_SWITCH_SPEED_TO_POS 10000L

#define TIMEOUT_WOBBLE_PERIODE 250
#define TIMEOUT_WAIT_BEFORE_OFF 250

#define SERVO_ANGLE_CENTER 90
#define SERVO_ANGLE_MAX 170
#define SERVO_ANGLE_MIN 10
#define SERVO_SPEED_MAX 200
#define SERVO_SPEED_MIN 10
#define SERVO_DEFAULT_CLOSED 140
#define SERVO_DEFAULT_THROWN 40
#define SERVO_DEFAULT_SPEED 20

typedef enum
{
    normalOperation = 0,
    setupAddress,
    setupServoPositionClose,
    setupServoPositionThrow,
    setupServoSpeed
}
DecoderStateMachine_t;

typedef enum 
{
    closePosition = 0,  // Close Turnout
    throwPosition,  // Throw Turnout
    endPosition  // Desired position of Turnout reached
}
Direction_t;

typedef enum
{
    unconfigured = 0,
    configured
}
Config_t;

typedef struct
{
    // config
    uint8_t posClosed;
    uint8_t posThrown;
    uint8_t speed;
    Config_t config;
    // working values
    uint8_t posCurrent;
    unsigned long tLastMove;
    Direction_t direction;
}
ServoSettings_t;
