#include <Arduino.h>
#include <cstdint>

#define IndicatorPinNum 12
#define IndicatorDigitMaskLen 10
#define IndicatorDigitsCount 4

// Mask that encodes a digit (at any position)
static uint8_t defaultDigitMask[IndicatorDigitMaskLen] = {
    0b11000000,
    0b11111001,
    0b10100100,
    0b10110000,
    0b10011001,
    0b10010010,
    0b10000010,
    0b11111000,
    0b10000000,
    0b10010000,
};

class Indicator {
public:
    Indicator(const int pin_nums_[IndicatorPinNum]/*first IndicatorDigitsCount are control anodes*/,
        const uint8_t digitMask_[IndicatorDigitMaskLen] = defaultDigitMask,
        TIM_TypeDef *instance = TIM3);
    ~Indicator();

    void SetDot(int which, int val);
    void ShowNumber(int num);
    void TurnOff();

    void TestDigitsLoop(int anode);

private:
    void displayIter();
    void displayDigitCodes(int n);
    void blackOut();
    const int *pin_nums;
    const uint8_t *digitMask;
    volatile bool dotHigh=false, dotLow=false;
    volatile int showNumber = -1;
    int iter=0;
    HardwareTimer *timer = nullptr;
};
