#include "Arduino.h"
#include "Wire.h"
#include "TM1637Display.h"
TM1637Display display(3, 2); // CLK, DIO
#include "SerialTransfer.h"

SerialTransfer transfer;
uint8_t buttonState;
bool buttonBannierePrev;

bool buttonSeqPrevUp;
bool buttonSeqPrevDown;

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

int16_t compteur = 0;

// #define DEBUG

void setup()
{
  Serial.begin(115200);
#ifndef DEBUG
  transfer.begin(Serial);
#endif

  buttonState = 1;
  buttonSeqPrevDown = false;
  buttonSeqPrevUp = false;
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
  pinMode(12, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  // Set brightness of the display
  display.setBrightness(4);
  // Serial.println("setup ");
}

void loop()
{
  message.x = analogRead(A3);
  message.y = map(analogRead(A2), 0, 1024, 1024, 0);
  message.z = analogRead(A0);
  message.compteur = compteur;

  if (digitalRead(8) == LOW)
  {
    compteur = compteur - 5;
  }

  if (digitalRead(9) == LOW)
  {
    compteur = compteur - 1;
  }

  if (digitalRead(10) == LOW)
  {
    compteur = compteur + 1;
  }

  if (digitalRead(11) == LOW)
  {
    compteur = compteur + 5;
  }

  if (digitalRead(4) == LOW)
  {
#ifdef DEBUG
    Serial.println("bouton 4");
#endif
  }

  if (digitalRead(4) == LOW && !buttonSeqPrevUp)
  {
    buttonState++;
    display.showNumberDec(buttonState);
  }
  buttonSeqPrevUp = (digitalRead(4) == LOW);

  if (digitalRead(5) == LOW && !buttonSeqPrevDown)
  {
    buttonState--;
    display.showNumberDec(buttonState);
  }
  buttonSeqPrevDown = (digitalRead(5) == LOW);

  if (digitalRead(4) == LOW)
  {
#ifdef DEBUG
    Serial.println("bouton 4");
#endif
  }

  if (digitalRead(5) == LOW)
  {
#ifdef DEBUG
    Serial.println("bouton 5");
#endif
  }

  if (digitalRead(11) == LOW)
  {
#ifdef DEBUG
    Serial.println("bouton 11");
#endif
  }

  if (digitalRead(12) == LOW)
  {
#ifdef DEBUG
    Serial.println("bouton 12");
#endif
  }

  if (digitalRead(13) == LOW)
  {
#ifdef DEBUG
    Serial.println("bouton 13");
#endif
  }

  message.cmdServoBanniere = (digitalRead(7) == LOW) || (message.x > 700); // Lacher la banniere

  if (digitalRead(7) == LOW)
  {

#ifdef DEBUG
    Serial.println("bouton 7");
#endif
  }

  if (digitalRead(8) == LOW)
  {
#ifdef DEBUG
    Serial.println("bouton 8");
#endif
  }

  if (digitalRead(9) == LOW)
  {
#ifdef DEBUG
    Serial.println("bouton 9");
#endif
  }

  if (digitalRead(10) == LOW)
  {
#ifdef DEBUG
    Serial.println("bouton 10");
#endif
  }

  if (digitalRead(11) == LOW)
  {
#ifdef DEBUG
    Serial.println("bouton 11");
#endif
  }

#ifdef DEBUG
  // Serial.print("boutonState: ");
  // Serial.println(buttonState);
#endif

  switch (buttonState)
  {
  case 1: // première etape
    message.ascBoites = 6600;
    message.cmdAimantInt = true;
    message.cmdAimantExt = true;
    message.cmdGliss = false;
    message.cmdPompe = false;
    message.ascPlanche = 0;
    message.cmdVanne = false;
    message.ascPlanche = 4500;
    break;
  case 2: // descente de planche du haut
    message.cmdPompe = true;
    message.ascPlanche = 5100;
    break;
  case 3: // remontée de la planche du haut
    message.ascPlanche = 0;
    message.cmdPompe = true;
    message.cmdServoPlanche = true;
    break;
  case 4: // retrait de la glissiere
    message.cmdGliss = true;
    break;
  case 5: // descente de la planche du bas
    message.cmdAimantExt = false;
    message.cmdServoPlanche = false;
    message.cmdGliss = false;
    break;
  case 6: // remontée des boites
    message.ascBoites = 1000;
    break;
  case 7: // remontée des boites
    message.ascBoites = 1500;
    break;
  case 8: // desactivation des aimants boite interieur
    message.cmdAimantInt = false;
    message.ascPlanche = 500;
    break;
  case 9: // lache la planche du haut 1
    message.cmdPompe = false;
    message.cmdVanne = true;
    break;
  case 10: // lache la planche du haut 2
    message.cmdVanne = false;
    buttonState = 1;
    display.showNumberDec(buttonState);
    break;
  }

  // Ensure compteur does not go below 0
  if (compteur < 0)
  {
    compteur = 0;
  }

#ifndef DEBUG
  // Send the message using SerialTransfer
  uint16_t sendSize = 0;
  sendSize = transfer.txObj(message, sendSize);
  transfer.sendData(sendSize);
#endif

  delay(50);
}
