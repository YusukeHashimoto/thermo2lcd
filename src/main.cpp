#include <LiquidCrystal.h>
#include <DHT.h>

#define DELAY_TIME 5000
#define USE_DEGREE_CELSIUS false

void rcvThermo(void);
void dispSerialMonitor(float, float);
void dispLCD(void);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int PIN_DHT = 2;
DHT dht( PIN_DHT, DHT11 );

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
}

void loop() {
  rcvThermo();
  dispLCD();
  dispSerialMonitor(temp, humi);
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
