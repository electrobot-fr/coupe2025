#include <AccelStepper.h>
#define CAM_DEBUG_LEVEL_ERROR 1
AccelStepper stepper2(1, 2, 5);
AccelStepper stepper3(1, 3, 6);


#define PERIMETRE_ROUE 188  // 6 * 3.14 mm
#define PAS_DANS_TOUR 1600
#define COEF_PAS_PAR_MM 8.5



// pour effectuer un tour de roue , il faut 1600 pas. Pour faire un quart de rotation avec une seule roue qui tourne, il faut 1120 pas soit 132mm



int chemin2[] = {0, 810, 0, 00, 0, 00, 0, 00, 0,}; // Tableau des positions
int tailleChemin2 = sizeof(chemin2) / sizeof(chemin2[0]); // Taille du tableau

int chemin3[] = {0, -810, 0, }; // Tableau des positions
int tailleChemin3 = sizeof(chemin3) / sizeof(chemin3[0]); // Taille du tableau
bool moteurArrete = false;  // Variable pour vérifier si le moteur doit être arrêté


  int currentPos = 0;


void setup() {
  
  Serial.begin(9600);
  Serial.println("Init");

  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  delay(2000);
// Déclaration de currentPos dans setup pour qu'il soit réinitialisé à chaque redémarrage


  stepper2.setMaxSpeed(2000.0);
  stepper2.setAcceleration(2000.0);
  stepper2.moveTo( COEF_PAS_PAR_MM * chemin2[0]);

  stepper3.setMaxSpeed(2000.0);
  stepper3.setAcceleration(2000.0);
  stepper3.moveTo( COEF_PAS_PAR_MM * chemin3[0]);
}


void loop() {
  // Si le moteur a atteint la position actuelle, on le déplace vers la prochaine
  if (stepper2.distanceToGo() == 0 && stepper3.distanceToGo() == 0 && !moteurArrete ) {
    
    
    // Si on a atteint la dernière position du tableau, on arrête le moteur
    if (currentPos < tailleChemin2 - 1) {
      currentPos++;
      stepper2.moveTo(stepper2.currentPosition()+chemin2[currentPos] * COEF_PAS_PAR_MM );  // Déplacer vers la prochaine position du tableau
      stepper3.moveTo(stepper3.currentPosition()+chemin3[currentPos] * COEF_PAS_PAR_MM );  // Déplacer vers la prochaine position du tableau
      Serial.print("Déplacement vers la position: ");
      Serial.println(chemin2[currentPos]);
    } else {
      // Si on est à la dernière position, on arrête le moteur
      moteurArrete = true;
      Serial.println("Dernière position atteinte. Le moteur s'arrête.");
    }
  }

  // Faire avancer le moteur si besoin
  if (!moteurArrete) {
    stepper2.run();
    stepper3.run();
  } else {
    // Une fois le moteur arrêté, on ne fait plus rien
    stepper2.stop();
    stepper3.stop();

  }
}
