#include "Console.h"

Console::Console() {}

Console::~Console() {}

void Console::startWobble()
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print("Wobble ... ");
#endif  
}

void Console::endWobble()
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.println("done");
#endif
}

void Console::displayIntro()
{
#ifdef DEBUG_SERVO_MOVEMENT   
    Serial.print("DCC Servo Decoder v");
    Serial.print(VERSION_MAJOR);
    Serial.print(".");
    Serial.print(VERSION_MINOR);
    Serial.println(" (https://github.com/mcfreis/)");
#endif
}

void Console::displayAddress(int address)
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print("Base-Address: ");
    Serial.println(address);
#endif
}

void Console::displayResetFactorySettings()
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.println("Resetting CVs to Factory Defaults");
#endif
}

void Console::displayWaitingForAddress()
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.println("Waiting for address ... ");
#endif
}

void Console::displaySwitchFromCloseToThrow(int servoIdx)
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print("Setup Servo (Thrown): ");
    Serial.println(servoIdx);
#endif
}

void Console::displaySetupClose(int servoIdx)
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print("Setup Servo (Closed): ");
    Serial.println(servoIdx);
#endif
}

void Console::displaySwitchToSpeed(int servoIdx)
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print("Setup Servo (Speed): ");
    Serial.println(servoIdx);
#endif
}

void Console::displayFinish()
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.println("Finished Setup");
#endif
}

void Console::displayReadSettings(int servoIdx, ServoSettings_t *settings)
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print("Read Servo: ");
    Serial.print(servoIdx);
    Serial.print(", Configured: ");
    Serial.print(settings->config);
    Serial.print(", Closed: ");
    Serial.print(settings->posClosed);
    Serial.print(", Thrown: ");
    Serial.print(settings->posThrown);
    Serial.print(", Current: ");
    Serial.print(settings->posCurrent);
    Serial.print(", Speed: ");
    Serial.println(settings->speed);
#endif
}

void Console::displaySaveSettings(int servoIdx, ServoSettings_t *settings)
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print("Save Servo: ");
    Serial.print(servoIdx);
    Serial.print(", Configured: ");
    Serial.print(settings->config);
    Serial.print(", Closed: ");
    Serial.print(settings->posClosed);
    Serial.print(", Thrown: ");
    Serial.print(settings->posThrown);
    Serial.print(", Current: ");
    Serial.print(settings->posCurrent);
    Serial.print(", Speed: ");
    Serial.println(settings->speed);
#endif
}

void Console::displayUpdatePosition(int servoIdx, ServoSettings_t *settings)
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print("Update Servo: ");
    Serial.print(servoIdx);
    Serial.print(", Current: ");
    Serial.println(settings->posCurrent);
#endif
}

void Console::displayTurnoutToggle(int servoIdx, ServoSettings_t *settings, Direction_t Direction)
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print((Direction == closePosition) ? "Close" : "Throw");
    Serial.print(" Servo: ");
    Serial.print(servoIdx);
    Serial.print(", to: ");
    Serial.print((Direction == closePosition) ? settings->posClosed : settings->posThrown);
    Serial.print(", speed: ");
    Serial.println(settings->speed);
#endif  
}

void Console::displayPositions(ServoSettings_t *settings)
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print("Closed: ");
    Serial.print(settings->posClosed);
    Serial.print(", Thrown: ");
    Serial.println(settings->posThrown);
#endif
}

void Console::displaySpeed(ServoSettings_t *settings)
{
#ifdef DEBUG_SERVO_MOVEMENT
    Serial.print("Speed: ");
    Serial.println(settings->speed);
#endif
}