#include <Servo.h>
#include <Arduino.h>
#include <AccelStepper.h>

// parametres pour moteurs pas à pas
#define CAM_DEBUG_LEVEL_ERROR 1
AccelStepper stepper2(1, 2, 5);
AccelStepper stepper3(1, 3, 6);

#define PERIMETRE_ROUE 188  // 6 * 3.14 mm
#define PAS_DANS_TOUR 1600
#define COEF_PAS_PAR_MM 8.5

// parametres pour servomoteur et interupteur

Servo monServo; // Création de l'objet Servo
const int pinServo = A0; // Broche de connexion du servomoteur (=Abort)
const int switchPin1 = A2; // Interrupteur démarrage robot, correspind à Resume
const int switchPin2 = A1; // choix équipe bleue ou jaune, correspond à Hold sur le CNC Shield


// Definition des chemins des deux roues
// De base, on rentre le chemin de l'équipe jaune. Si c'est l'équipe bleue, alors il faut interchanger chemin2 et 3 et mettre un signe - devant

// int chemin2[] = {0, 100, 132, 100, 0, 100, 0, 100, 0, 300, 264}; // Tableau des positions//
int chemin2[] = {0, -1150, 0,-80, 365}; // Tableau des positions
//int chemin2[] = {150, 0, 0, ,0}; // Tableau des positions

int tailleChemin2 = sizeof(chemin2) / sizeof(chemin2[0]); // Taille du tableau

// int chemin3[] = {0, -100, 0, -100, -132, -100, -132, -100, -132, -300, 0}; // Tableau des positions
int chemin3[] = {0, 1150, 132,+80, -365}; // Tableau des positions
// int chemin3[] = {125, 0, 0, 0, 0}; // Tableau des positions

int tailleChemin3 = sizeof(chemin3) / sizeof(chemin3[0]); // Taille du tableau
bool moteurArrete = false;  // Variable pour vérifier si le moteur doit être arrêté

bool depart = true; // Variable pour reperer si on est sur la position de départ
bool jaune = true; // Par défaut on suppose que c'est l'équipe jaune
int currentPos = 0;
long attente = 88000;

void setup() {
  pinMode(switchPin1, INPUT_PULLUP); // initialisation pin démarrage robot
  pinMode(switchPin2, INPUT_PULLUP); // initialisation bouton choix équipe
  monServo.attach(pinServo); // Attache le servomoteur à la broche
  monServo.write(90); // Position initiale au centre (90°)
  delay(200); // Pause pour stabiliser le servo

  pinMode(8, OUTPUT);   // quelle est l'utilité?
  digitalWrite(8, LOW);
  delay(200);

  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(2000.0);
  stepper2.moveTo( COEF_PAS_PAR_MM * chemin2[0]);

  stepper3.setMaxSpeed(2000.0);
  stepper3.setAcceleration(2000.0);
  stepper3.moveTo( COEF_PAS_PAR_MM * chemin3[0]);
  currentPos= 0;
}

void loop() {
  bool isMoveTriggered = digitalRead(switchPin1) == LOW;
  if (isMoveTriggered) {
    if (depart){
     delay(attente);
     depart = false;
     jaune = digitalRead(switchPin2)== LOW;
    }
    // Si le moteur a atteint la position actuelle, on le déplace vers la prochaine
    if (stepper2.distanceToGo() == 0 && stepper3.distanceToGo() == 0 && !moteurArrete ) {
      // Si on a atteint la dernière position du tableau, on arrête le moteur
      if (currentPos < tailleChemin2 - 1) {
        currentPos++;
        if (jaune){
          stepper2.moveTo(stepper2.currentPosition()+chemin2[currentPos] * COEF_PAS_PAR_MM );  // Déplacer vers la prochaine position du tableau
          stepper3.moveTo(stepper3.currentPosition()+chemin3[currentPos] * COEF_PAS_PAR_MM );  // Déplacer vers la prochaine position du tableau
        } else{
          stepper2.moveTo(stepper2.currentPosition()-chemin3[currentPos] * COEF_PAS_PAR_MM );  // Déplacer vers la prochaine position du tableau
          stepper3.moveTo(stepper3.currentPosition()-chemin2[currentPos] * COEF_PAS_PAR_MM );  // Déplacer vers la prochaine position du tableau
        }
                
      } else {
        // Si on est à la dernière position, on arrête le moteur
        moteurArrete = true;
      }
    }
    // Faire avancer le moteur si besoin
    if (!moteurArrete) {
     stepper2.run();
     stepper3.run();
    } else {
     // Une fois atteint la fin du déplacement, on arrete les moteurs pas à pas et on lance le servomoteur
     stepper2.stop();
     stepper3.stop();
     // Déplacement à +45° (135° au total)
     monServo.write(135);
     delay(1000); // Pause d'une seconde
     // Déplacement à -45° (45° au total)
     monServo.write(45);
     delay(1000); // Pause d'une seconde
    }

  }
} 
   

