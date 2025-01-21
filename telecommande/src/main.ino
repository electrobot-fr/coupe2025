#include "Arduino.h"
#include "SerialTransfer.h"
#include "Wire.h"

SerialTransfer transfer;

struct __attribute__((packed)) STRUCT {
  int16_t x;
  int16_t y;
  bool a;
  bool b;
} message;

void setup() {
  Serial.begin(115200);
  transfer.begin(Serial);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}

void loop() {
  message.x = analogRead(A0);
  message.y = analogRead(A1);
  message.b = true;
  message.a = true;

  uint16_t sendSize = 0;
  sendSize = transfer.txObj(message, sendSize);
  transfer.sendData(sendSize);

  delay(10);
}
