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
  bool cmdGlissGauche;      // Glissière gauche : 0: retracter / 1: deployer
  bool cmdGlissDroit;       // Glissière droite
//  bool cmdAimantIntGauche;  // Pince aimant interieur gauche : 0: détacher / 1: attacher
//  bool cmdAimantExtGauche;  // Pince aimant exterieur gauche
//  bool cmdAimantIntDroit;   // Pince aimant interieur droit
//  bool cmdAimantExtDroit;   // Pince aimant exterieur droit
//  bool cmdPompe;            // Commande Pompe : 0: Off / 1: On
//  bool cmdVanne;            // Commande Electrovanne : 0: Off / 1: On
} message;

SerialTransfer transfer;

TM1637Display display(3, 2); // CLK, DIO

ServoEasing servoGlissDroit;
ServoEasing servoGlissGauche;
Servo servoAscDroit;
Servo servoAscGauche;
Servo servoAimantIntGauche;
Servo servoAimantExtGauche;
Servo servoAimantIntDroit;
Servo servoAimantExtDroit;

#define POMPE 30
#define VALVE 31

AccelStepper stepperX(1, 34, 36);
AccelStepper stepperY(1, 35, 37);
#define ENABLE_STEPPER 38

void setup() {
  Serial.begin(115200); // port USB
  Serial1.begin(115200); // port 19
  transfer.begin(Serial1);

  Serial.println("Debug port");

  servoGlissGauche.attach(7);
  servoGlissDroit.attach(6);  
  servoAscGauche.attach(8);
  servoAscDroit.attach(9);
  servoAimantIntGauche.attach(10);
  servoAimantExtGauche.attach(11);
  servoAimantIntDroit.attach(12);
  servoAimantExtDroit.attach(13);

  pinMode(POMPE, OUTPUT); // pompe
  pinMode(VALVE, OUTPUT); // valve

  servoGlissGauche.setSpeed(80);
  servoGlissDroit.setSpeed(80);

  servoGlissDroit.write(90);
  servoGlissGauche.write(90);
  servoAscDroit.write(150);
  servoAscGauche.write(60);
  servoAimantIntGauche.write(90);
  servoAimantExtGauche.write(90);
  servoAimantIntDroit.write(90);
  servoAimantExtDroit.write(0);

  digitalWrite(POMPE, LOW);
  digitalWrite(VALVE, LOW);

  pinMode(ENABLE_STEPPER, OUTPUT);
  digitalWrite(ENABLE_STEPPER, LOW);
   
  stepperX.setMaxSpeed(7000.0);
  stepperX.setAcceleration(6000.0);
  stepperY.setMaxSpeed(7000.0);
  stepperY.setAcceleration(6000.0);

  display.setBrightness(4);
}

void loop() {
  String cmd = "";

  if (transfer.available())
  {
    uint16_t recSize = 0;
    recSize = transfer.rxObj(message, recSize);

    Serial.print("Received: x = ");
    Serial.print(message.x);
    Serial.print(", y = ");
    Serial.println(message.y);

    // if (message.x >= 1000) {
    //   servoAimantIntGauche.write(0);
    //   servoAimantExtGauche.write(0);
    //   servoAimantIntDroit.write(0);
    //   servoAimantExtDroit.write(90);
    // } else {
    //   servoAimantIntGauche.write(90);
    //   servoAimantExtGauche.write(90);
    //   servoAimantIntDroit.write(90);
    //   servoAimantExtDroit.write(0);
    // }

    // if (message.y >= 1000) {
    //   servoAscDroit.write(90);
    //   servoAscGauche.write(120);
    // } else {
    //   servoAscDroit.write(150);
    //   servoAscGauche.write(60);
    // }

    if (message.z >= 1000) {
      servoGlissDroit.startEaseTo(35);
      servoGlissGauche.startEaseTo(140);
    } else {
      servoGlissDroit.startEaseTo(90);
      servoGlissGauche.startEaseTo(90);
    }

    if (message.y >= 1000) {
      digitalWrite(POMPE, HIGH);
      // digitalWrite(VALVE, HIGH);
    } else {
      digitalWrite(POMPE, LOW);
      digitalWrite(VALVE, LOW);
    }

    if (message.x >= 1000) {
      stepperX.moveTo(1000);
    } else {
      stepperX.moveTo(0);
    }
    // Display the updated value on the TM1637 display
    // display.showNumberDec(message.compteur);

    // memcpy(&prevMsg, &msg, sizeof(messageType));
  }

  servoGlissGauche.update();
  servoGlissDroit.update();

  stepperX.run();
  stepperY.run();

}
