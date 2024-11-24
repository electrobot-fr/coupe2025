
//Ports de commande du moteur B
int motor1Pin1 = 8;
int motor1Pin2 = 9;
int enable1Pin = 13;
int motor2Pin1 = 12;
int motor2Pin2 = 11;
int enable2Pin = 10;
int motor3Pin1 = 7;
int motor3Pin2 = 6;
int enable3Pin = 5;
int motor4Pin1 = 3;
int motor4Pin2 = 2;
int enable4Pin = 4;

#define pinA1 A0
#define pinA2 A1 
// Vitesse du moteur
int state = 0;

void setup() {
    // Configuration des ports en mode "sortie"
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(enable1Pin, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    pinMode(enable2Pin, OUTPUT);
    pinMode(motor3Pin1, OUTPUT);
    pinMode(motor3Pin2, OUTPUT);
    pinMode(enable3Pin, OUTPUT);
    pinMode(motor4Pin1, OUTPUT);
    pinMode(motor4Pin2, OUTPUT);
    pinMode(enable4Pin, OUTPUT);
    pinMode(pinA1, INPUT);
    pinMode(pinA2, INPUT);
    // Initialisation du port série
    Serial.begin(9600);
}
 void loop() {
  int a1 = analogRead(pinA1);
   state = map(a1, 0, 1023, -255, 255);
  
   // if (Serial.available() > 0)
    {
      // Lecture de l'entier passé au port série
      //state = Serial.parseInt();

      //
      // Sens du mouvement
      //
      if (state > 0) // avant
      {
        digitalWrite(motor1Pin1, HIGH); 
        digitalWrite(motor1Pin2, LOW);
        Serial.print("Avant ");
        Serial.println(state);
        digitalWrite(motor2Pin1, HIGH); 
        digitalWrite(motor2Pin2, LOW);
        digitalWrite(motor3Pin1, HIGH); 
        digitalWrite(motor3Pin2, LOW);
        digitalWrite(motor4Pin1, HIGH); 
        digitalWrite(motor4Pin2, LOW);
      }
      else if (state < 0) // arrière
      {
        digitalWrite(motor1Pin1, LOW); 
        digitalWrite(motor1Pin2, HIGH);
        digitalWrite(motor2Pin1, LOW); 
        digitalWrite(motor2Pin2, HIGH);
        digitalWrite(motor3Pin1, LOW); 
        digitalWrite(motor3Pin2, HIGH);
        digitalWrite(motor4Pin1, LOW); 
        digitalWrite(motor4Pin2, HIGH);
        Serial.print("Avant ");
        Serial.println(state);
        Serial.print("Arriere ");
        Serial.println(state);
      }
      else // Stop (freinage)
      {
        digitalWrite(motor1Pin1, HIGH); 
        digitalWrite(motor1Pin2, HIGH);
        digitalWrite(motor2Pin1, HIGH); 
        digitalWrite(motor2Pin2, HIGH);
        Serial.println("Stop");
      }

      //
      // Vitesse du mouvement
      //
      analogWrite(enable1Pin, abs(state));
      analogWrite(enable2Pin, abs(state));
      analogWrite(enable3Pin, abs(state));
      analogWrite(enable4Pin, abs(state));
    }
    delay(100);
    Serial.println(state);
     //int a2 = analogRead(pinA2);
     //a2 = map(a2, 0, 1023, 0, 180);
    //Serial.println(a2);
}