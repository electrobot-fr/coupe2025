#include "Arduino.h"
#include <SerialTransfer.h>
#include "Wire.h"

SerialTransfer transfer;

int motArGPin1 = 8;
int motArGPin2 = 9;
int motArGena = 13;

int motArDPin1 = 12;
int motArDPin2 = 11;
int motArDena = 10;

int motAvDPin1 = 7;
int motAvDPin2 = 6;
int motAvDena = 5;

int motAvGPin1 = 3;
int motAvGPin2 = 2;
int motAvGena = 4;

#define pinA0 A0
#define pinA1 A1

struct __attribute__((packed)) STRUCT
{
  int16_t x;
  int16_t y;
  bool a;
  bool b;
} message;

void setup()
{
  // Configuration des ports en mode "sortie"
  pinMode(motArDPin1, OUTPUT);
  pinMode(motArDPin2, OUTPUT);
  pinMode(motArDena, OUTPUT);
  pinMode(motArGPin1, OUTPUT);
  pinMode(motArGPin2, OUTPUT);
  pinMode(motArGena, OUTPUT);

  pinMode(motAvDPin1, OUTPUT);
  pinMode(motAvDPin2, OUTPUT);
  pinMode(motAvDena, OUTPUT);
  pinMode(motAvGPin1, OUTPUT);
  pinMode(motAvGPin2, OUTPUT);
  pinMode(motAvGena, OUTPUT);

  pinMode(pinA0, INPUT);
  pinMode(pinA1, INPUT);

  // Initialisation du port série
  Serial.begin(115200);
  transfer.begin(Serial);
}

void loop()
{
  if (transfer.available())
  {
    uint16_t recSize = 0;
    recSize = transfer.rxObj(message, recSize);

    int a0 = message.x;
    int a1 = message.y;
    int state = map(a1, 0, 1023, -250, 250);
    int state2 = map(a0, 0, 1023, -250, 250);

    if ((state > 0) && (abs(state) > abs(state2))) // avant
    {
      digitalWrite(motArGPin1, HIGH);
      digitalWrite(motArGPin2, LOW);

      digitalWrite(motArDPin1, HIGH);
      digitalWrite(motArDPin2, LOW);

      digitalWrite(motAvDPin1, HIGH);
      digitalWrite(motAvDPin2, LOW);

      digitalWrite(motAvGPin1, HIGH);
      digitalWrite(motAvGPin2, LOW);
    }
    else if ((state < 0) && (abs(state) > abs(state2))) // arrière
    {
      digitalWrite(motArDPin1, LOW);
      digitalWrite(motArDPin2, HIGH);

      digitalWrite(motArGPin1, LOW);
      digitalWrite(motArGPin2, HIGH);

      digitalWrite(motAvDPin1, LOW);
      digitalWrite(motAvDPin2, HIGH);

      digitalWrite(motAvGPin1, LOW);
      digitalWrite(motAvGPin2, HIGH);
    }
    else if ((state2 < 0) && (abs(state2) > abs(state))) // droite
    {
      digitalWrite(motAvDPin1, LOW);
      digitalWrite(motAvDPin2, HIGH);

      digitalWrite(motAvGPin1, HIGH);
      digitalWrite(motAvGPin2, LOW);

      digitalWrite(motArDPin1, HIGH);
      digitalWrite(motArDPin2, LOW);

      digitalWrite(motArGPin1, LOW);
      digitalWrite(motArGPin2, HIGH);
    }
    else if ((state2 > 0) && (abs(state2) > abs(state))) // gauche
    {
      digitalWrite(motAvDPin1, HIGH);
      digitalWrite(motAvDPin2, LOW);

      digitalWrite(motAvGPin1, LOW);
      digitalWrite(motAvGPin2, HIGH);

      digitalWrite(motArDPin1, LOW);
      digitalWrite(motArDPin2, HIGH);

      digitalWrite(motArGPin1, HIGH);
      digitalWrite(motArGPin2, LOW);
    }
    else // Stop (freinage)
    {
      digitalWrite(motArDPin1, HIGH);
      digitalWrite(motArDPin2, HIGH);

      digitalWrite(motArGPin1, HIGH);
      digitalWrite(motArGPin2, HIGH);

      digitalWrite(motAvDPin1, HIGH);
      digitalWrite(motAvDPin2, HIGH);

      digitalWrite(motAvGPin1, HIGH);
      digitalWrite(motAvGPin2, HIGH);
    }

    if (abs(state2) > abs(state))
    {
      state = state2;
    }

    analogWrite(motArGena, abs(state));
    analogWrite(motArDena, abs(state));
    analogWrite(motAvDena, abs(state));
    analogWrite(motAvGena, abs(state));
  }
}