#pragma once

class ButtonTimer {

private:
    unsigned long tPress;
    unsigned long tFormerPress;

public:
    ButtonTimer();
    ~ButtonTimer();

    void reset();
    void press();
    unsigned long delta();
    unsigned long since();
};
