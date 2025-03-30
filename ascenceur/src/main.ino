#include <Servo.h>
#include <string.h>
#include <Arduino.h>
#include "Wire.h"
#include "TM1637Display.h"
TM1637Display display(3, 2); // CLK, DIO
#include "SerialTransfer.h"

Servo servoPince;
SerialTransfer transfer;
typedef struct __attribute__((packed)) STRUCT {
  int16_t x; // base roulante: avance / recule
  int16_t y; // base roulante: moteur gauche / droite
  int16_t z; // base roulante: rotation gauche / droite
  //  int16_t compteur; // compteur pour affichage
  bool cmdGlissGauche;      // Glissière gauche : 0: retracter / 1: deployer
  bool cmdGlissDroit;       // Glissière droite
//  bool cmdAimantIntGauche;  // Pince aimant interieur gauche : 0: détacher / 1: attacher
//  bool cmdAimantExtGauche;  // Pince aimant exterieur gauche
//  bool cmdAimantIntDroit;   // Pince aimant interieur droit
//  bool cmdAimantExtDroit;   // Pince aimant exterieur droit
//  bool cmdPompe;            // Commande Pompe : 0: Off / 1: On
//  bool cmdVanne;            // Commande Electrovanne : 0: Off / 1: On
} messageType;

messageType msg;
messageType prevMsg;
int16_t compteur = 0;

Servo servoGlissDroit;
Servo servoGlissGauche;
Servo servoAimantIntGauche;
Servo servoAimantExtGauche;
Servo servoAimantIntDroit;
Servo servoAimantExtDroit;

void setup() {
  Serial.begin(115200);
  transfer.begin(Serial);
  
  servoGlissGauche.attach(9);
  servoGlissDroit.attach(10);
  servoAimantIntGauche.attach(8);
  servoAimantExtGauche.attach(11);
  servoAimantIntDroit.attach(12);
  servoAimantExtDroit.attach(13);

  //Serial.println("Servo Control Initialized");

  servoGlissDroit.write(90);
  servoGlissGauche.write(90);
  servoAimantIntGauche.write(90);
  servoAimantExtGauche.write(90);
  servoAimantIntDroit.write(90);
  servoAimantExtDroit.write(90);

  // Set brightness of the display
  display.setBrightness(4);
}

void loop() {
  String cmd = "";

  if (transfer.available())
  {
    uint16_t recSize = 0;
    recSize = transfer.rxObj(msg, recSize);

    if(msg.cmdGlissDroit != prevMsg.cmdGlissDroit)
    {
      if(msg.cmdGlissDroit)
        servoGlissDroit.write(90);
      else
        servoGlissDroit.write(90+20);
    }

    // Display the updated value on the TM1637 display
    // display.showNumberDec(message.compteur);

    memcpy(&prevMsg, &msg, sizeof(messageType));
  }

}
