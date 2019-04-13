#include "DHT.h"
#include <Wire.h>
#include "SSD1306Wire.h"

#define LED D4

DHT dht;
SSD1306Wire display(0x3c, D2, D3);

void setup() {
  dht.setup(D1);
  display.init();
  
  pinMode(LED, OUTPUT);
}

void drawProgressBar(int time) {
  int wait = (time / 100);

  for (int counter = 0; counter <= 100; counter++) {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawProgressBar(0, 32, 120, 10, counter);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 10, String(counter) + "%");
    display.display();
    delay(wait);
  }
}

void drawSensorValue(float value, String caracter) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  display.drawString(64, 15, String(value, 0) + caracter);
  display.display();
}

void checkLimitValue(int value, int maximum) {
  if (value >= maximum) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

void loop() {
  drawProgressBar(dht.getMinimumSamplingPeriod());
  
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();

  checkLimitValue(temperature, 30);

  drawSensorValue(temperature, " °C");
  delay(10000);
  drawSensorValue(humidity, " %");
  delay(5000);
}