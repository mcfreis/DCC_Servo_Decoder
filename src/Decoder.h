#include <Servo.h>
#include <NmraDcc.h>

#include "DccServoDecoder.h"
#include "ButtonTimer.h"
#include "Console.h"

#pragma once

class Decoder
{
private:
    void checkIfSetupMode();
    void resetCVs();
    void initServos();
    void saveServo(int servoIdx);
    void configureCycleToSpeed();
    void configureCycleThroughServos();

    void wobbleServo(int iterations, int servoIdx);
    void startSetup();
    void setServoToPosition(int servoIdx);
    void testServoSpeed(int servoIdx);

    void configureServoPosition(Direction_t Direction);
    void configureServoSpeed(Direction_t Direction);
    void setTurnoutDirection(uint16_t Addr, Direction_t Direction);
    void moveServoToPosition(int servoIdx, Direction_t Direction);

    ServoSettings_t servoSettings[SERVO_COUNT];  // working copy of the current settings
    Servo servo[SERVO_COUNT]; // create servo object to control a servo
    int pwmPin[SERVO_COUNT] = PWM_PINS;
    int powerPin[SERVO_COUNT] = POWER_PINS;
    DecoderStateMachine_t decoderSM = normalOperation;
    bool doResetEEProm = false;
    int currentServoIdx;
    ButtonTimer setupButton;
    Direction_t lastDirection;
    int baseAddress;
    Console console;
    bool currentServoModified;

public:
    Decoder();
    ~Decoder();

    void setupDCC();   
    void process();
    void processTurnoutData(uint16_t Addr, Direction_t Direction);

    NmraDcc Dcc;
};

extern Decoder decoder;
