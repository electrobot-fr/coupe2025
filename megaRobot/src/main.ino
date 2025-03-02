#include "Arduino.h"
#include <SerialTransfer.h>
#include "Wire.h"

SerialTransfer transfer;

#define MaxPwm  (150)

int motArGPin1 = 8;
int motArGPin2 = 9;
int motArGena = 11;

int motArDPin1 = 12;
int motArDPin2 = 13;
int motArDena = 10;

int motAvDPin1 = 7;
int motAvDPin2 = 6;
int motAvDena = 5;

int motAvGPin1 = 4;
int motAvGPin2 = 2;
int motAvGena = 3;

struct __attribute__((packed)) STRUCT
{
  int16_t x;
  int16_t y;
  int16_t z;
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

    int cmd = 0;
    int x = map(message.x, 0, 1023, -MaxPwm, MaxPwm);
    int y = map(message.y, 0, 1023, -MaxPwm, MaxPwm);
    int z = map(message.z, 0, 1023, -MaxPwm, MaxPwm);

    if ((abs(z) > abs(y)) && (abs(z) > abs(x)))
    {
      Rotate(z);
      cmd = z;
    }
    else
    {
      Drive(y, x);
      if (abs(y) > abs(x))
      {
        cmd = y;
      }
      else
      {
        cmd = x;
      }
    }

    analogWrite(motArGena, abs(cmd));
    analogWrite(motArDena, abs(cmd));
    analogWrite(motAvDena, abs(cmd));
    analogWrite(motAvGena, abs(cmd));
  }
}

void StopMotors() // Stop (freinage)
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

void Drive(int y, int x)
{
    if ((y > 0) && (abs(y) > abs(x))) // avant
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
    else if ((y < 0) && (abs(y) > abs(x))) // arrière
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
    else if ((x < 0) && (abs(x) > abs(y))) // droite
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
    else if ((x > 0) && (abs(x) > abs(y))) // gauche
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
    else StopMotors();
}

void Rotate(int z)
{
    if (z < 0) // rotation droite
    {
      digitalWrite(motAvDPin1, HIGH);
      digitalWrite(motAvDPin2, LOW);

      digitalWrite(motAvGPin1, LOW);
      digitalWrite(motAvGPin2, HIGH);

      digitalWrite(motArDPin1, HIGH);
      digitalWrite(motArDPin2, LOW);

      digitalWrite(motArGPin1, LOW);
      digitalWrite(motArGPin2, HIGH);
    }
    else if (z > 0)// rotation gauche
    {
      digitalWrite(motAvDPin1, LOW);
      digitalWrite(motAvDPin2, HIGH);

      digitalWrite(motAvGPin1, HIGH);
      digitalWrite(motAvGPin2, LOW);

      digitalWrite(motArDPin1, LOW);
      digitalWrite(motArDPin2, HIGH);

      digitalWrite(motArGPin1, HIGH);
      digitalWrite(motArGPin2, LOW);
    }
    else StopMotors();
}