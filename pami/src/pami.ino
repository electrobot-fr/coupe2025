#include <AccelStepper.h>

AccelStepper stepper2(1, 2, 5);
AccelStepper stepper3(1, 3, 6);

void setup() {
  Serial.begin(9600);
  Serial.println("Init");

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);

  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(2000.0);
  stepper2.moveTo(1000);

  stepper3.setMaxSpeed(2000.0);
  stepper3.setAcceleration(2000.0);
  stepper3.moveTo(1000);
}

void loop() {
  // Change direction at the limits
  if (stepper3.distanceToGo() == 0) {
    Serial.println("Changement de sens...");
    stepper2.moveTo(-stepper2.currentPosition());
    stepper3.moveTo(-stepper3.currentPosition());
  }

  stepper2.run();
  stepper3.run();
}