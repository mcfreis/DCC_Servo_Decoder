#include <Arduino.h>

#include "ButtonTimer.h"

ButtonTimer::ButtonTimer()
{
    tPress = tFormerPress = 0;
}

ButtonTimer::~ButtonTimer()
{

}

void ButtonTimer::reset()
{
    tFormerPress = tPress = millis();
}

void ButtonTimer::press()
{
    tFormerPress = tPress;
    tPress = millis();
}

unsigned long ButtonTimer::delta()
{
    return tPress - tFormerPress;
}

unsigned long ButtonTimer::since()
{
    return millis() - tPress;
}