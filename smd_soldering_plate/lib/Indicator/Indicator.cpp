#include <functional>
#include <Indicator.hpp>


Indicator::Indicator(const int pin_nums_[IndicatorPinNum], const uint8_t digitMask_[IndicatorDigitMaskLen], TIM_TypeDef *instance)
        : pin_nums(pin_nums_), digitMask(digitMask_) {
    for(int i=0; i<IndicatorPinNum; i++)
        pinMode(pin_nums[i], OUTPUT);
    TurnOff();
    timer = new HardwareTimer(instance);
    timer->pause();
    timer->setOverflow(500, HERTZ_FORMAT);
    timer->attachInterrupt(std::bind(&Indicator::displayIter, this));
    timer->resume();
}

Indicator::~Indicator() {
    if( timer ) delete timer;
}

void Indicator::displayIter() {
    if( showNumber < 0 ) {
        return;
    }
    digitalWrite(pin_nums[iter], LOW);
    if( ++iter == IndicatorDigitsCount )
        iter = 0;

    int num = showNumber;
    for(int i=0; i<iter; i++) {
        num /= 10;
    }
    int digit = num%10;
    if( digit != 0 || iter == 0 || num>=10 ) {
        displayDigitCodes(digit);
        digitalWrite(pin_nums[IndicatorDigitsCount-iter-1], HIGH);
    } else {
        blackOut();
    }

    if( iter==1 && dotHigh ) {
        digitalWrite(pin_nums[2], HIGH);
        digitalWrite(pin_nums[4], LOW);
    }
    if( iter==0 && dotLow ) {
        digitalWrite(pin_nums[3], HIGH);
        digitalWrite(pin_nums[4], LOW);
    }
}

void Indicator::SetDot(int which, int val) {
    if(which == HIGH) {
        dotHigh = val;
    }else{
        dotLow = val;
    }
}

void Indicator::ShowNumber(int num) {
    showNumber = num;
}

void Indicator::TurnOff() {
    ShowNumber(-1);
    blackOut();
}

void Indicator::blackOut() {
    for(int i=0; i<IndicatorDigitsCount; i++) {
        digitalWrite(pin_nums[i], LOW);
    }
    for(int i=IndicatorDigitsCount; i<IndicatorPinNum; i++) {
        digitalWrite(pin_nums[i], HIGH);
    }
}

void Indicator::displayDigitCodes(int n) {
    uint16_t bin = digitMask[n];
    for(int j=IndicatorPinNum-1; j>=0; j--) {
        digitalWrite(pin_nums[j], (bin & 1) ? HIGH : LOW);
        bin >>= 1;
    }
}

void Indicator::TestDigitsLoop(int anode) {
    while(1) {
        for(int i=0; i<10; i++) {
            // For each digit we display
            Serial.print("Showing number:");
            Serial.println(i);
            displayDigitCodes(i);
            digitalWrite(anode, HIGH);
            delay(500);
        }
    }
}
