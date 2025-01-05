#include <Servo.h>
#include <Arduino.h>

Servo servoPince;
Servo servoAcs;
Servo servoPince2;
Servo servoAcs2;

void setup() {
  Serial.begin(9600); 
  servoPince.attach(9);  
  servoAcs.attach(10);
  servoPince2.attach(8);  
  servoAcs2.attach(11);  
  
  Serial.println("Servo Control Initialized");

  servoAcs.write(90);
  servoPince.write(45);
  servoAcs2.write(90);
  servoPince2.write(45);
}

void loop() {
  if (Serial.available() > 0) {
    int cmd = Serial.parseInt();

    if (cmd == 1) {
      servoAcs.write(60);
      servoAcs2.write(60);
    }
    else if (cmd == 2) {
      servoAcs.write(120);
      servoAcs2.write(120);
    }
    else if (cmd == 3) {
      servoPince.write(1);
      servoPince2.write(1);
    }
    else if (cmd == 4) {
      servoPince.write(90);
      servoPince2.write(90);
    }
    
    // Ensure the angle is within the valid range (0-180 degrees)
    // if (angle != 0) {
    //   servoAcs.write(angle);  // Move the servo to the specified angle
    //   Serial.print("Moving servo to: ");
    //   Serial.println(angle);
    // } else {
    //   Serial.println("Invalid input! Please enter a value between 0 and 180.");
    // }
  }
}

