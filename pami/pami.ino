#include <AccelStepper.h>
 
// Define some steppers and the pins the will use
AccelStepper stepper2(1, 2, 5);
AccelStepper stepper3(1, 3, 6);
 
void setup()
{  
    pinMode(8, OUTPUT);
  digitalWrite(8, LOW);

    stepper2.setMaxSpeed(2000.0);
    stepper2.setAcceleration(2000.0);
    stepper2.moveTo(10000);
    
    stepper3.setMaxSpeed(2000.0);
    stepper3.setAcceleration(2000.0);
    stepper3.moveTo(-10000); 
}
 
void loop()
{
    // Change direction at the limits
    if (stepper2.distanceToGo() == 0) {
      //   stepper2.moveTo(-stepper2.currentPosition());
      //  stepper3.moveTo(-stepper3.currentPosition());
    }

    stepper2.run();
    stepper3.run();
}