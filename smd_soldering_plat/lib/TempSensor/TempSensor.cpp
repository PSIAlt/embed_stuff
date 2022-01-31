#include <TempSensor.hpp>

/*
3950 NTC 25C

100k - 25C
10.6k - 85C
6.7k - 100C
3.85k - 120C
1.77k - 150C
0.99k - 175C
0.58k - 200C
0.36k - 225C
0.23k - 250C
0.153k - 275C
0.122k - 290C
0.105k - 300C
*/

#define B 3950 // B-коэффициент
#define SERIAL_R 5126 // сопротивление последовательного резистора
#define THERMISTOR_R 100000 // номинальное сопротивления термистора, 100 кОм
#define NOMINAL_T 25 // номинальная температура (при которой TR = 100 кОм)

#define SerialWrite 0

volatile int TempSensor::curChannel = 0;
HardwareTimer *TempSensor::timer = nullptr;

TempSensor::TempSensor() {
}

TempSensor::TempSensor(TempSensor &&other) {
    pin = other.pin;
    timer = other.timer;
    channel = other.channel;
    tempearature = other.tempearature;

    other.timer = nullptr;
    other.pin = -1;
}

volatile int led=0;

TempSensor::TempSensor(int pin_, TIM_TypeDef *instance) {
    if( curChannel >= TIMER_CHANNELS ) {
        Serial.println("Too many instances of TempSensor!");
        return;
    }
    pin = pin_;
    channel = ++curChannel;

    pinMode(PC13, OUTPUT);
    pinMode(pin, INPUT_ANALOG);

    if( !timer) timer = new HardwareTimer(instance);
    timer->pause();
    //if( channel==1 ) 
        timer->setOverflow(4, HERTZ_FORMAT);
    timer->pauseChannel(channel);
    timer->setMode(channel, TIMER_OUTPUT_COMPARE_ACTIVE);
    timer->setCaptureCompare(channel, 20, TICK_COMPARE_FORMAT);
    static int logDivider=0;
    timer->attachInterrupt(channel, [this]() {
        analogReadResolution(12);
        int t = analogRead(this->pin);
        analogReadResolution(10);

        // https://robotclass.ru/tutorials/arduino-thermistor-100k/
        float tr = 4095.0 / t - 1;
        tr = SERIAL_R / tr;
        if( SerialWrite && (logDivider++)%100==0 ) {
            Serial.print("Ch=");
            Serial.print(this->channel);
            Serial.print(", R=");
            Serial.print(tr);
            Serial.print(", t=");
        }

        float steinhart;
        steinhart = tr / THERMISTOR_R; // (R/Ro)
        steinhart = log(steinhart); // ln(R/Ro)
        steinhart /= B; // 1/B * ln(R/Ro)
        steinhart += 1.0 / (NOMINAL_T + 273.15); // + (1/To)
        steinhart = 1.0 / steinhart; // Invert
        steinhart -= 273.15; 
        if( SerialWrite ) Serial.println(steinhart);
        this->tempearature = steinhart;

        if( channel == 1) {
            digitalWrite(PC13, led);
            led=!led;
        }
    });

    timer->resume();
}

TempSensor::~TempSensor() {
    if( timer ) {
        timer->pauseChannel(channel);
    }
}
