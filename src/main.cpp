#include "DHT.h"
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "SSD1306Wire.h"

#define LED D4

DHT dht;
SSD1306Wire display(0x3c, D2, D3);

WiFiClient client;
PubSubClient mqqtClient(client);

const char* SSID_WIFI = "";
const char* PASSWORD_WIFI = "";
const char* DOJOT_URL = "";
const char* TOPIC_PUBLISH = "";

void setup() {
  dht.setup(D1);
  display.init();
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  mqqtClient.setServer(DOJOT_URL, 1883);
}

void wirelessConnection() {
  if (WiFi.status() == WL_CONNECTED){
    digitalWrite(LED_BUILTIN, LOW);
    return;
  }

  WiFi.begin(SSID_WIFI, PASSWORD_WIFI);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print(".");
  }

  Serial.println("Connected to WIFI: ");
  Serial.println(WiFi.localIP());
}

void publishDataToDojot(float value) {
  char payload[50];
  char data[20];
  dtostrf(value, 5, 0, data);
  sprintf(payload, "{\"temperature\" : %s }", data);

  mqqtClient.connect("nodemcu");
  
  if (mqqtClient.connected()) {
    Serial.println("Sending temperature " + String(value, 0) + " °C to Dojot");
    mqqtClient.publish(TOPIC_PUBLISH, payload);
  } else {
    Serial.println("MQTT Broker disconnected, data lost...");
  }  
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
  wirelessConnection();
  drawProgressBar(dht.getMinimumSamplingPeriod());

  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();

  checkLimitValue(temperature, 26);
  publishDataToDojot(temperature);

  drawSensorValue(temperature, " °C");
  delay(20000);
  drawSensorValue(humidity, " %");
  delay(5000);

  mqqtClient.loop();
}