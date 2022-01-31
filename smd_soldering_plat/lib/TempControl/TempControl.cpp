#include "TempControl.hpp"

TempControl::TempControl(int _pin) : pin(_pin), regulator(2.0, 0.001, 0.10, 10) {
  pinMode(pin, OUTPUT);
  regulator.setDirection(NORMAL); // направление регулирования (NORMAL/REVERSE). ПО УМОЛЧАНИЮ СТОИТ NORMAL
  regulator.setLimits(0, 255);    // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
  SetWantTemp(wantTemperature);
}

void TempControl::SetCurrentTemp(int v) {
    currentTemperature = v;
    regulator.input  = v;
    checkAction();
}

int TempControl::GetCurrentTemp() {
    return currentTemperature;
}

void TempControl::SetWantTemp(int v) {
    wantTemperature = v;
    regulator.setpoint = v;
    checkAction();
}

int TempControl::GetWantTemp() {
    return wantTemperature;
}

void TempControl::SetEnabled(bool v) {
    heatEnabled = v;
    checkAction();
}

int TempControl::GetEnabled() {
    return heatEnabled;
}

void TempControl::checkAction() {
    if( !heatEnabled ) {
        analogWrite(pin, 0);
        return;
    }
    analogWriteFrequency(500);
    analogWrite(pin, regulator.getResultTimer());
    analogWriteFrequency(PWM_FREQUENCY);
}
