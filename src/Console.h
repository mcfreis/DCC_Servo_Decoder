#include "DccServoDecoder.h"

#pragma once

class Console {

public:
    Console();
    ~Console();

    void startWobble();
    void endWobble();

    void displayIntro();
    void displayAddress(int address);
    void displayResetFactorySettings();
    void displayWaitingForAddress();
    void displaySwitchFromCloseToThrow(int servoIdx, ServoSettings_t *setting);
    void displaySetupClose(int servoIdx, ServoSettings_t *setting);
    void displaySwitchToSpeed(int servoIdx, ServoSettings_t *setting);
    void displayFinish();

    void displayReadSettings(int servoIdx, ServoSettings_t * settings);
    void displaySaveSettings(int servoIdx, ServoSettings_t *settings);
    void displayUpdatePosition(int servoIdx, ServoSettings_t *settings);

    void displayTurnoutToggle(int servoIdx, ServoSettings_t *settings, Direction_t Direction);

    void displayPositions(ServoSettings_t *settings);
    void displaySpeed(ServoSettings_t *settings);
};
