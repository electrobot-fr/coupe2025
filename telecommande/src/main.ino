#include "Arduino.h"
#include "Wire.h"
#include "TM1637Display.h"
TM1637Display display(3, 2); // CLK, DIO
#include "SerialTransfer.h"

SerialTransfer transfer;

struct __attribute__((packed)) STRUCT {
  int16_t x;
  int16_t y;
  int16_t z;
//  int16_t compteur;
  bool a;
  bool b;
} message;

int16_t compteur = 0;

void setup() {
  Serial.begin(115200);
  transfer.begin(Serial);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);

  // Initialize buttons with INPUT_PULLUP mode
  pinMode(8, INPUT_PULLUP);  // Button 1 to add 1
  pinMode(9, INPUT_PULLUP);  // Button 2 to subtract 1
  pinMode(10, INPUT_PULLUP); // Button 3 to add 5
  pinMode(11, INPUT_PULLUP); // Button 4 to subtract 5
  // Set brightness of the display
  display.setBrightness(4);
}

void loop() {
  message.x = analogRead(A3);
  message.y = analogRead(A2);
  message.z = analogRead(A0);
  //message.compteur = compteur;
  message.b = true;
  message.a = true;

  // Send the message using SerialTransfer
  uint16_t sendSize = 0;
  sendSize = transfer.txObj(message, sendSize);
  transfer.sendData(sendSize);

  delay(10);

//  // Read button states and update compteur value
  int buttonState1 = digitalRead(8);
  if (buttonState1 == LOW) {
    compteur = compteur + 1;
    delay(150); // Debounce delay
  }

  int buttonState2 = digitalRead(9);
  if (buttonState2 == LOW) {
    compteur = compteur - 1;
    delay(150); // Debounce delay
  }

  int buttonState3 = digitalRead(10);
  if (buttonState3 == LOW) {
    compteur = compteur + 5;
    delay(200); // Debounce delay
  }

  int buttonState4 = digitalRead(11);
  if (buttonState4 == LOW) {
    compteur = compteur - 5;
    delay(200); // Debounce delay
  }

  // Ensure compteur does not go below 0
  if (compteur < 0) compteur = 0;

  // Display the updated value on the TM1637 display
  display.showNumberDec(compteur);
}
