#include "Arduino.h"
#include "Wire.h"
#include "TM1637Display.h"
TM1637Display display(3, 2); // CLK, DIO
#include "SerialTransfer.h"

SerialTransfer transfer;
uint8_t buttonState;
bool buttonSeq;
bool buttonSeqPrev;

struct __attribute__((packed)) STRUCT {
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

int16_t compteur = 0;

void setup() {
  Serial.begin(115200);
  transfer.begin(Serial);

  buttonState = 1;
  buttonSeqPrev = false;
  buttonSeq = false;
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);

  // Initialize buttons with INPUT_PULLUP mode
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);  // Button 1 to add 1
  pinMode(9, INPUT_PULLUP);  // Button 2 to subtract 1
  pinMode(10, INPUT_PULLUP); // Button 3 to add 5
  pinMode(11, INPUT_PULLUP); // Button 4 to subtract 5
  // Set brightness of the display
  display.setBrightness(4);
  //Serial.println("setup ");
}

void loop() {
  message.x = analogRead(A3);
  message.y = analogRead(A2);
  message.z = analogRead(A0);
  message.compteur = compteur;

  // Send the message using SerialTransfer
  uint16_t sendSize = 0;
  sendSize = transfer.txObj(message, sendSize);
  transfer.sendData(sendSize);

  delay(10);

//  // Read button states and update compteur value
  int buttonState1 = digitalRead(8);
  if (buttonState1 == LOW) {
    compteur = compteur + 1;
    //Serial.println("bouton 1");
    delay(150); // Debounce delay
  }
  
  int buttonState2 = digitalRead(9);
  if (buttonState2 == LOW) {
    compteur = compteur - 1;
    //Serial.println("bouton 2");
    delay(150); // Debounce delay
  }

  int buttonState3 = digitalRead(10);
  if (buttonState3 == LOW) {
    compteur = compteur + 5;
    //Serial.println("bouton 4");
    delay(200); // Debounce delay
  }

  int buttonState4 = digitalRead(11);
  if (buttonState4 == LOW) {
    //Serial.println("bouton 3");
    compteur = compteur - 5;
    delay(200); // Debounce delay
  }

  buttonSeq = (digitalRead(5) == LOW);
 
  if (digitalRead(4) == LOW) {
    //Serial.println("bouton 5");
    buttonSeq = true;
  }

  if (digitalRead(5) == LOW) {
    //Serial.println("bouton 6");
  }

  if (digitalRead(6) == LOW) {
   // //Serial.println("bouton 7");
  }

  if (digitalRead(7) == LOW) {
    //Serial.println("bouton 8");
  }

  if(buttonSeq && !buttonSeqPrev)
  {
    buttonState++;
  }

  switch(buttonState)
  {
    case 1: // première etape
      message.ascBoites = 6600;
      message.cmdAimantInt = true;
      message.cmdAimantExt = true;
      message.cmdGliss = false;
      message.cmdPompe = false;
      message.ascPlanche = 0;
      message.cmdVanne = false;
      break;

    case 2: // descente de planche du haut
      message.cmdPompe = true;
      message.ascPlanche = 5000;
      break;

    case 3: // remontée de la planche du haut
      message.ascPlanche = 0;
      message.cmdPompe = true;
      break;

    case 4: // remontée de la planche du bas
        message.cmdServoPlanche = true;
      break;

    case 5: // retrait de la glissiere
        message.cmdGliss = true;
      break;

    case 6: // descente de la planche du bas
        message.cmdAimantExt = false;
        message.cmdServoPlanche = false;
        break;

    case 7: // remontée de la planche du bas
      message.cmdServoPlanche = true;
      break;

    case 8: // retrait de la glissiere
      message.cmdGliss = false;
      break;
    case 9: // remontée des boites
      message.ascBoites = 1000;
      break;
    case 10: // remontée des boites
      message.ascBoites = 1500;
      break;
    case 11: // desactivation des aimants boite interieur
      message.cmdAimantInt = false;
      break;
    case 12: // desactivation des aimants boite interieur
      message.ascPlanche = 500;
      break;
    case 13: // lache la planche du haut 1
      message.cmdPompe = false;
      message.cmdVanne = true;
      break;
    case 14: // lache la planche du haut 2
      message.cmdVanne = false;
      break;
    default:
      buttonState = 1;
  }
  buttonSeqPrev = buttonSeq;
  // Ensure compteur does not go below 0
  if (compteur < 0) compteur = 0;

  // Display the updated value on the TM1637 display
  display.showNumberDec(compteur);
}
