#include <Arduino.h>
#include "GyverPID.h"

class TempControl {
public:
    TempControl(int _pin);
    void SetCurrentTemp(int v);
    int GetCurrentTemp();
    void SetWantTemp(int v);
    int GetWantTemp();
    void SetEnabled(bool v);
    int GetEnabled();

private:
    void checkAction();

    int pin;
    bool heatEnabled = false;
    int currentTemperature=-1, wantTemperature=150;
    GyverPID regulator;
};
