#include <LiquidCrystal.h>
#include <DHT.h>
#include <SR04.h>

#define DELAY_TIME 5000
#define USE_DEGREE_CELSIUS false

#define BACKLIGHT_PIN 4
#define TRIG_PIN 5
#define ECHO_PIN 6

void rcvThermo(void);
void dispSerialMonitor(float, float);
void dispLCD(void);
void setBackLight(void);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int PIN_DHT = 2;
DHT dht( PIN_DHT, DHT11 );
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
  dht.begin();

  pinMode(BACKLIGHT_PIN, OUTPUT);
  digitalWrite(BACKLIGHT_PIN, HIGH);
}

void loop() {
  rcvThermo();
  dispLCD();
  dispSerialMonitor(temp, humi);
  setBackLight();
  delay(DELAY_TIME);
}

void rcvThermo(void){
  humi = dht.readHumidity();
  temp = dht.readTemperature(USE_DEGREE_CELSIUS);

  if (isnan(temp) || isnan(humi)) {
    Serial.println("ERROR");
  }
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

void setBackLight(void){
  long a;
  a = sr04.Distance();
  if(abs(a - dist) > 3) {
    moveLessCount = 0;
    backLightOnOff = true;
    dist = a;
  } else {
    moveLessCount++;
    if(moveLessCount > 12) {
      backLightOnOff = false;
    }
  }
  Serial.println(dist);
//  backLightOnOff = (a < 20) ? true : false;
  digitalWrite(BACKLIGHT_PIN, backLightOnOff);
}