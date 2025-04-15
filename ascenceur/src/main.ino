#include <Servo.h>
#include <string.h>
#include <Arduino.h>
#include "Wire.h"
#include "TM1637Display.h"
#include "SerialTransfer.h"
#include <ServoEasing.hpp>
#include <AccelStepper.h>


struct __attribute__((packed)) STRUCT
{
  int16_t x;
  int16_t y;
  int16_t z;
  bool cmdGliss;         // Glissière : 0: retracter / 1: deployer
  bool cmdAimantInt;     // Pince aimant interieur 0: détacher / 1: attacher
  bool cmdAimantExt;     // Pince aimant exterieur
  bool cmdPompe;         // Commande Pompe : 0: Off / 1: On
  bool cmdVanne;         // Commande Electrovanne : 0: Off / 1: On
  bool cmdServoPlanche;  // Lever les planches
  bool cmdServoBanniere; // Lacher la banniere
  int16_t ascPlanche;    // Position de l'ascenceur des planches
  int16_t ascBoites;     // Position de l'ascenceur des boites
  uint8_t compteur;      // Compteur
} message;

SerialTransfer transfer;

TM1637Display display(51, 50); // CLK, DIO

ServoEasing servoGlissDroit;
ServoEasing servoGlissGauche;
Servo servoAscDroit;
Servo servoAscGauche;
Servo servoAimantIntGauche;
Servo servoAimantExtGauche;
Servo servoAimantIntDroit;
Servo servoAimantExtDroit;
Servo servoBanniere;

#define POMPE 30
#define VALVE 31

AccelStepper stepperX(1, 34, 36);
AccelStepper stepperY(1, 35, 37);
#define ENABLE_STEPPER 38

void setup() {
  Serial.begin(115200); // port USB
  Serial1.begin(115200); // port 19
  transfer.begin(Serial1);

  // Serial.println("Debug port");

  servoGlissGauche.attach(7);
  servoGlissDroit.attach(6);  
  servoAscGauche.attach(8);
  servoAscDroit.attach(9);
  servoAimantIntGauche.attach(10);
  servoAimantExtGauche.attach(11);
  servoAimantIntDroit.attach(12);
  servoAimantExtDroit.attach(13);
  servoBanniere.attach(46);

  pinMode(POMPE, OUTPUT); // pompe
  pinMode(VALVE, OUTPUT); // valve

  servoGlissGauche.setSpeed(80);
  servoGlissDroit.setSpeed(80);

  servoGlissDroit.write(80);
  servoGlissGauche.write(100);
  servoAscDroit.write(150);
  servoAscGauche.write(60);
  servoAimantIntGauche.write(90);
  servoAimantExtGauche.write(90);
  servoAimantIntDroit.write(90);
  servoAimantExtDroit.write(0);
  servoBanniere.write(15);

  digitalWrite(POMPE, LOW);
  digitalWrite(VALVE, LOW);

  pinMode(ENABLE_STEPPER, OUTPUT);
  digitalWrite(ENABLE_STEPPER, LOW);
   
  stepperX.setMaxSpeed(7000.0);
  stepperX.setAcceleration(6000.0);
  stepperY.setMaxSpeed(7000.0);
  stepperY.setAcceleration(6000.0);

  display.setBrightness(4);
  display.showNumberDec(42);
}

int compteur = 0;

void loop() {
  String cmd = "";

  if (transfer.available())
  {
    uint16_t recSize = 0;
    recSize = transfer.rxObj(message, recSize);

    // Serial.print("Received: x = ");
    // Serial.print(message.x);
    // Serial.print(", y = ");
    // Serial.println(message.y);

    if (message.cmdAimantInt) {
      servoAimantIntGauche.write(0);
      servoAimantIntDroit.write(0);
    } else {
      servoAimantIntGauche.write(90);
      servoAimantIntDroit.write(90);
    }

    if (message.cmdAimantExt) {
      servoAimantExtGauche.write(0);
      servoAimantExtDroit.write(90);
    } else {
      servoAimantExtGauche.write(90);
      servoAimantExtDroit.write(0);
    }

    if (message.cmdServoPlanche) {
      servoAscDroit.write(90);
      servoAscGauche.write(120);
    } else {
      servoAscDroit.write(150);
      servoAscGauche.write(60);
    }

    if (message.cmdGliss) {
      servoGlissDroit.startEaseTo(25);
      servoGlissGauche.startEaseTo(150);
    } else {
      servoGlissDroit.startEaseTo(80);
      servoGlissGauche.startEaseTo(100);
    }

    if (message.cmdPompe) {
      digitalWrite(POMPE, HIGH);
    } else {
      digitalWrite(POMPE, LOW);
    }

    if (message.cmdVanne) {
      digitalWrite(VALVE, HIGH);
    } else {
      digitalWrite(VALVE, LOW);
    }

    if (message.cmdServoBanniere) {
      servoBanniere.write(90);
    } else {
      servoBanniere.write(15);
    }

    stepperY.moveTo(message.ascBoites);
    stepperX.moveTo(message.ascPlanche);

    if (compteur != message.compteur) {
      display.showNumberDec(message.compteur);
      compteur = message.compteur;
    }
  }

  servoGlissGauche.update();
  servoGlissDroit.update();

  stepperX.run();
  stepperY.run();

}
