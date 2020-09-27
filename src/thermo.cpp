#include <DHT.h>

#define USE_DEGREE_CELSIUS false

static DHT *dht;
static float temp;
static float humi;

void initThermo(unsigned char pin_DHT) {
    dht = new DHT(pin_DHT, DHT11);
  dht->begin();
}

void receiveThermo(void){
  humi = dht->readHumidity();
  temp = dht->readTemperature(USE_DEGREE_CELSIUS);

  if (isnan(temp) || isnan(humi)) {
    //Serial.println("ERROR");
  }
}

float getTempreture(void) {
    return temp;
}

float getHumidity(void) {
    return humi;
}