#include <LiquidCrystal.h>
#include <SR04.h>

#include "thermo.h"

#define DELAY_TIME 5000

#define BACKLIGHT_PIN 4
#define TRIG_PIN 5
#define ECHO_PIN 6

#define POWER_SAVE_MODE

void rcvThermo(void);
void dispSerialMonitor(float, float);
void dispLCD(void);
void judgeBackLight(void);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int PIN_DHT = 2;
bool backLightOnOff = true;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long dist;
int moveLessCount = 0;

float temp = 0;
float humi = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Temp:       C");
  lcd.setCursor(0,1);
  lcd.print("Humi:       %");

  Serial.begin(9600);
  Serial.println("DHT11 start");
  initThermo(PIN_DHT);

  pinMode(BACKLIGHT_PIN, OUTPUT);
  digitalWrite(BACKLIGHT_PIN, HIGH);
}

void loop() {
  receiveThermo();
  temp = getTempreture();
  humi = getHumidity();
  dispLCD();
  dispSerialMonitor(temp, humi);
  judgeBackLight();
  delay(DELAY_TIME);
}

void dispLCD(void) {
  lcd.setCursor(6, 0);
  lcd.print(temp);

  lcd.setCursor(6, 1);
  lcd.print(humi);
}

void dispSerialMonitor(float temp, float humi){
  String s = "Temp: ";
  s += String(temp, 1);
  s += "[C] Humidity: ";
  s += String(humi, 1);
  s += "[%]";

  Serial.println(s);
}

void judgeBackLight(void){
#ifdef POWER_SAVE_MODE
  long a;
  a = sr04.Distance();
  if(abs(a - dist) > 3) {
    moveLessCount = 0;
    backLightOnOff = true;
    dist = a;
  } else {
    moveLessCount++;
    if(moveLessCount > 12 && dist > 0) {
      backLightOnOff = false;
    }
  }
  Serial.println(dist);
//  backLightOnOff = (a < 20) ? true : false;
  digitalWrite(BACKLIGHT_PIN, backLightOnOff);
#endif  //POWER_SAVE_MODE
}