#include <Arduino.h>

class TempSensor {
public:
    TempSensor();
    TempSensor(int pin_, TIM_TypeDef *instance = TIM2);
    ~TempSensor();
    TempSensor(TempSensor &&other);

    float GetTemperature() { return tempearature; }

private:
    static volatile int curChannel;
    int pin=-1, channel;
    static HardwareTimer *timer;
    volatile float tempearature=0;
};
