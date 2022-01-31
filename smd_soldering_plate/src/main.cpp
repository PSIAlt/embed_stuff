#include <Arduino.h>

#include <TempSensor.hpp>
#include <Indicator.hpp>
#include <RotaryEncoder.h>
#include <TempControl.hpp>

#define PIN_SENS1 PA_7
#define PIN_SENS2 PA_6
#define HEAT_PIN PA2
#define ZUMMER_PIN PB11

TempSensor *sens1;
TempSensor *sens2;
Indicator *ind;

RotaryEncoder encoder(PB7, PB8, PB9);
TempControl tc(HEAT_PIN);

const int pin_nums[] = {
  PB12, PB13, PB14, PB15, PA8, PA9, PA10, PA15, PB3, PB4, PB5, PB6
};

void encoderISR() {
  encoder.readAB();
}

void encoderButtonISR() {
  encoder.readPushButton();
}

void setup() {
  pinMode(ZUMMER_PIN, OUTPUT);
  digitalWrite(ZUMMER_PIN, LOW);

  Serial.begin(115200);
  delay(500);
  Serial.println("Started");
  sens1 = new TempSensor(PIN_SENS1);
  sens2 = new TempSensor(PIN_SENS2);

  ind = new Indicator(pin_nums);

  encoder.begin();                                                           //set encoders pins as input & enable built-in pullup resistors
  attachInterrupt(digitalPinToInterrupt(PB7),  encoderISR,       CHANGE);  //call encoderISR()       every high->low or low->high changes
  attachInterrupt(digitalPinToInterrupt(PB8),  encoderISR,       CHANGE);  //call encoderISR()       every high->low or low->high changes
  attachInterrupt(digitalPinToInterrupt(PB9), encoderButtonISR, FALLING); //call encoderButtonISR() every high->low              changes
  encoder.setPosition( tc.GetWantTemp() );
}

void loop() {
  // Handle rotations
  int newPos = encoder.getPosition();

  uint32_t currtime = millis();
  static uint32_t lastRotate = millis();
  if ( tc.GetWantTemp() != newPos) {

    int diff = newPos - tc.GetWantTemp();
    if( (currtime - lastRotate) <= 10 ) {
      newPos += 4*diff; //quad speed
      encoder.setPosition(newPos);
    }
    Serial.printf("newPos=%i\n", newPos);
    tc.SetWantTemp(newPos);
    lastRotate = currtime;
  }

  // Handle button
  if( encoder.getPushButton() == true ) {
    static uint32_t lastKeyPress = 0;
    if( (currtime - lastKeyPress) > 500 ) {
      tc.SetEnabled( !tc.GetEnabled() );
      lastKeyPress = currtime;

      digitalWrite(ZUMMER_PIN, HIGH);
      delay(25);
      digitalWrite(ZUMMER_PIN, LOW);
    }
  }

  // Handle display
  if( tc.GetCurrentTemp()==-1 || !tc.GetEnabled() || (currtime - lastRotate) < 1000 ) {
    ind->ShowNumber(newPos);
    ind->SetDot(HIGH, 1);
    ind->SetDot(LOW, 1);
  } else {
    ind->ShowNumber( tc.GetCurrentTemp() );
    ind->SetDot(HIGH, 0);
    ind->SetDot(LOW, 0);
  }

  // Update temp controller
  {
    int maxtemp = int(sens1->GetTemperature());
    int t2 = int(sens2->GetTemperature());
    if( t2 > maxtemp ) maxtemp = t2;
    int avg3temp = (sens1->GetTemperature() + sens2->GetTemperature() + maxtemp) / 3;

    tc.SetCurrentTemp( avg3temp );
  }
}
