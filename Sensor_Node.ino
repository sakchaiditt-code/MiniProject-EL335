// ---------- Sensor Node (ตัวลูก) ----------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "DHT.h"

// ----- ขาเซนเซอร์เดิมของคุณ -----
#define DHTPIN   2
#define DHTTYPE  DHT11
#define LDR_PIN  A0
#define SOIL_PIN A1

// ----- ขา nRF24L01 เดิมของคุณ -----
#define CE_PIN   9
#define CSN_PIN  10
RF24 radio(CE_PIN, CSN_PIN);

// RF Address ต้องตรงกับตัวแม่
const byte address[6] = "NODE1";

// Struct ต้องตรงกับตัวแม่
struct SensorData {
  uint16_t ldr;
  uint16_t soil;
  float temp;
  float hum;
};

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Serial.println("Sensor Node started.");
}

void loop() {
  SensorData data;

  data.ldr  = analogRead(LDR_PIN);
  data.soil = analogRead(SOIL_PIN);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    data.temp = -1.0;
    data.hum  = -1.0;
  } else {
    data.temp = t;
    data.hum  = h;
  }

  bool ok = radio.write(&data, sizeof(data));

  Serial.print("Send -> LDR:");
  Serial.print(data.ldr);
  Serial.print(" SOIL:");
  Serial.print(data.soil);
  Serial.print(" T:");
  Serial.print(data.temp);
  Serial.print(" H:");
  Serial.print(data.hum);
  Serial.print(" status:");
  Serial.println(ok ? "OK" : "FAIL");

  delay(2000);
}
