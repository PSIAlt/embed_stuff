#include <Arduino.h> 
#include <LiquidCrystal.h> 
#include "DHT.h" 
 
#define PIN_FAN PD6  
#define DHTPIN A5 
#define LIGHT_PIN PD7 
#define BTN_SHIFT 8 
 
// Period in seconds 
uint64_t ON_PERIOD = (14*3600UL); 
uint64_t OFF_PERIOD = (10*3600UL); 
 
DHT dht(DHTPIN, DHT11); 
 
// initialize the library by associating any needed LCD interface pin 
// with the arduino pin number it is connected to 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 
int fan_is_on = 0, fan_on_times = 0; 
int light_on=1; 
uint64_t light_millis_start=0; 
 
void setup() { 
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2); 
  Serial.begin(9600); 
  dht.begin(); 
  pinMode(PIN_FAN, OUTPUT); 
  pinMode(LIGHT_PIN, OUTPUT); 
  pinMode(BTN_SHIFT, INPUT_PULLUP); 
    delay(2000); // 2 секунды задержки 
} 
 
void light_control() { 
  uint64_t want_pause = light_on ? ON_PERIOD : OFF_PERIOD; 
  uint64_t s = millis() / 1000; 
  if( (light_millis_start+want_pause) < s || digitalRead(BTN_SHIFT) == 0 ) { 
    light_on = !light_on; 
    light_millis_start=s; 
  } 
  // Loop to manage the light 
  digitalWrite(LIGHT_PIN, light_on ? HIGH: LOW); 
  lcd.setCursor(0, 0); 
  lcd.print("S:"); 
   //lcd.print( uint32_t(want_pause) ); 
  lcd.print( uint32_t(((light_millis_start+want_pause) - s)/60) ); 
  if( light_on ) { 
    lcd.print("*"); 
  } 
  lcd.print("  "); 
} 
 
void loop() { 
  float h = dht.readHumidity(); //Измеряем влажность 
  float t = dht.readTemperature(); //Измеряем температуру 
  if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу 
    Serial.println("Ошибка считывания"); 
    return; 
  } 
  Serial.print("Влажность: "); 
  Serial.print(h); 
  Serial.print(" %\t"); 
 
  Serial.print("Температура: "); 
  Serial.print(t); 
  Serial.println(" *C "); //Вывод показателей на экран 
 
 
   
  lcd.setCursor(0, 1); 
  lcd.print(h); 
 
  lcd.setCursor(6, 1); 
  lcd.print(t); 
 
  if( t > 30 ) { 
    if( fan_is_on==0) { 
      fan_on_times++; 
    } 
    digitalWrite(PIN_FAN, HIGH);  
    lcd.setCursor(12, 1); 
    lcd.print("*"); 
    lcd.print(fan_on_times); 
    fan_is_on=1; 
  } else { 
    digitalWrite(PIN_FAN, LOW ); 
    lcd.setCursor(12, 1); 
    lcd.print(" "); 
    lcd.print(fan_on_times); 
    fan_is_on=0; 
  } 
 
  light_control(); 
}
