#include <Wire.h>
#include "SSD1306Wire.h"

SSD1306Wire display(0x3c, D1, D2);

void setup() {
  display.init();
}

void drawProgressBar() {
  for (int counter = 0; counter <= 100; counter++) {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawProgressBar(0, 32, 120, 10, counter);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 10, String(counter) + "%");
    display.display();
    delay(20);
  }
}

void drawTemperature(int value) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  display.drawString(64, 15, String(value) + " °C");
  display.display();
}

int getActualTemperature() {
  return 24;
}

void loop() {
  drawProgressBar();
  drawTemperature(getActualTemperature());
  delay(20000);
}