/*
  The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 */

// include the library code:
#include <LiquidCrystal.h>
#include <DHT.h>

#define DELAY_TIME 5000
#define DEBUG_MODE

void rcvThermo(void);

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int PIN_DHT = 2;
DHT dht( PIN_DHT, DHT11 );

float temp = 0;
float humi = 0;
bool error = false;

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
  lcd.setCursor(6, 0);
  lcd.print(temp);

  lcd.setCursor(6, 1);
  lcd.print(humi);

  rcvThermo();
}

void rcvThermo(){
  delay(DELAY_TIME);

  const bool isFahrenheit = false;
  float percentHumidity = dht.readHumidity();
  float temperature = dht.readTemperature( isFahrenheit );

  if (isnan(percentHumidity) || isnan(temperature)) {
    Serial.println("ERROR");
    Serial.println(percentHumidity);
    return;
  }

  float heatIndex = dht.computeHeatIndex(
    temperature, 
    percentHumidity, 
    isFahrenheit);

  String s = "Temp: ";
  s += String(temperature, 1);
  s += "[C] Humidity: ";
  s += String(percentHumidity, 1);
  s += "[%] HI: ";
  s += String(heatIndex, 1);

  Serial.println(s);

  temp = temperature;
  humi = percentHumidity;
}

void disp_serial_monitor(float temp, float humi){
  
}
