#include <Arduino.h>

#pragma once

#define CV_SERVO_CLOSE 30
#define CV_SERVO_THROW 31
#define CV_SERVO_POSITION 32
#define CV_SERVO_SPEED 33
#define CV_SERVO_CONFIG 34

#define CV_SERVO_OFFSET(cv_idx, servo) (cv_idx + (servo * 5))

struct CVPair
{
    uint16_t CV;
    uint8_t Value;
};

extern struct CVPair FactoryDefaultCVs[45];
