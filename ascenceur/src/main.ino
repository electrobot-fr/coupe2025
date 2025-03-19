// #include <Arduino.h>

// void setup()
// {
//   Serial.begin(9600);
//   Serial.println("Init");
//   Serial.setTimeout(100);

//   pinMode(9, OUTPUT); // pompe
//   pinMode(5, OUTPUT); // valve
// }

// void loop()
// {
//   if (Serial.available() > 0)
//   {
//     int cmd = Serial.parseInt();
//     if (cmd == 1) // aspire
//     {
//       digitalWrite(9, HIGH);
//       digitalWrite(5, LOW);
//     }
//     else if (cmd == 2) // relache
//     {
//       digitalWrite(9, LOW);
//       digitalWrite(5, HIGH);
//       delay(500);
//       digitalWrite(5, LOW);
//     }
//   }
// }

// #include <AccelStepper.h>
// #include <Arduino.h>

// AccelStepper stepperX(1, 2, 5);
// AccelStepper stepperY(1, 3, 6);

// void setup()
// {
//   Serial.begin(115200);
//   Serial.println("Stepper init");

//   pinMode(8, OUTPUT);
//   digitalWrite(8, LOW);

//   stepperX.setMaxSpeed(7000.0);
//   stepperX.setAcceleration(6000.0);

//   stepperY.setMaxSpeed(7000.0);
//   stepperY.setAcceleration(6000.0);

//   Serial.setTimeout(100);
// }

// void loop()
// {
//   if (Serial.available() > 0)
//   {
//     String cmd = Serial.readStringUntil('\n');
//     cmd.trim(); 

//     if (cmd == "RAZX")
//     {
//       stepperX.setCurrentPosition(0);
//       Serial.println("RAZ stepper X");
//     }
//     else if (cmd == "X0")
//     {
//       stepperX.moveTo(0);
//       Serial.println("stepper X 0");
//     }
//     else if (cmd == "X1")
//     {
//       stepperX.moveTo(4000);
//       Serial.println("stepper X 4000");
//     }
//     else if (cmd == "RAZY")
//     {
//       stepperY.setCurrentPosition(0);
//       Serial.println("RAZ stepper Y");
//     }
//     else if (cmd == "Y0")
//     {
//       stepperY.moveTo(0);
//       Serial.println("stepper Y 0");
//     }
//     else if (cmd == "Y1")
//     {
//       stepperY.moveTo(4000);
//       Serial.println("stepper Y 4000");
//     }
//   }
//   stepperX.run();
//   stepperY.run();
// }

// Pinces
#include <Servo.h>
#include <Arduino.h>

Servo servoPince;
Servo servoAcs;
Servo servoPince2;
Servo servoAcs2;

void setup() {
  Serial.begin(115200);
  servoPince.attach(9);
  servoAcs.attach(10);
  servoPince2.attach(8);
  servoAcs2.attach(11);

  Serial.println("Servo Control Initialized");

  servoAcs.write(90);
  servoPince.write(90);
  servoAcs2.write(90);
  servoPince2.write(90);
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim(); 

    if (cmd == "ACS0") {
      servoAcs.write(90);
      servoAcs2.write(90);
      Serial.println("1");
    }
    else if (cmd == "ACS1") {
      servoAcs.write(90 - 20);
      servoAcs2.write(90 + 20);
      Serial.println("2");
    }
    else if (cmd == "PIN0") {
      servoPince.write(90);
      servoPince2.write(90);
      Serial.println("3");
    }
    else if (cmd == "PIN1") {
      servoPince.write(90- 20);
      servoPince2.write(90 + 20);
      Serial.println("4");
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
