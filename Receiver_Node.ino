// ---------- Receiver Node (ตัวแม่) ----------
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ----- ขา nRF24L01 เดิมของคุณ -----
#define CE_PIN   9
#define CSN_PIN  10

RF24 radio(CE_PIN, CSN_PIN);

// ต้องตรงกับตัวลูก
const byte address[6] = "NODE1";

struct SensorData {
  uint16_t ldr;
  uint16_t soil;
  float temp;
  float hum;
};

void setup() {
  Serial.begin(9600);  // Dashboard ใช้ 9600
  delay(200);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  Serial.println("Receiver Node Ready");
}

void loop() {
  if (radio.available()) {
    SensorData data;
    radio.read(&data, sizeof(data));

    // ส่งไปหน้าเว็บแบบ JSON
    Serial.print("{\"ldr\":");
    Serial.print(data.ldr);
    Serial.print(",\"soil\":");
    Serial.print(data.soil);
    Serial.print(",\"temp\":");
    Serial.print(data.temp, 1);
    Serial.print(",\"hum\":");
    Serial.print(data.hum, 1);
    Serial.println("}");
  }

  delay(50);
}
