
//Ports de commande du moteur B
int motAvDPin1 = 8;
int motAvDPin2 = 9;
int motAvDena = 13;
bool motAvDactive = false;

int motArDPin1 = 12;
int motArDPin2 = 11;
int motArDena= 10;
bool motArDactive = false;

int motAvGPin1 = 7;
int motAvGPin2 = 6;
int motAvGena = 5;
bool motAvGactive = true;

int motArGPin1 = 3;
int motArGPin2 = 2;
int motArGena = 4;
bool motArGactive = true;

#define pinA0 A0
#define pinA1 A1 
// Vitesse du moteur
int state = 0;
int state2 = 0;

void setup() {
    // Configuration des ports en mode "sortie"
    if(motArDactive == true)
   {  
      pinMode(motArDPin1, OUTPUT);
      pinMode(motArDPin2, OUTPUT);
      pinMode(motArDena, OUTPUT); 
    }
    if(motAvDactive == true)
    {
      pinMode(motAvDPin1, OUTPUT);
      pinMode(motAvDPin2, OUTPUT);
      pinMode(motAvDena, OUTPUT);
   }
    
    
    pinMode(motAvGPin1, OUTPUT);
    pinMode(motAvGPin2, OUTPUT);
    pinMode(motAvGena, OUTPUT);
    pinMode(motArGPin1, OUTPUT);
    pinMode(motArGPin2, OUTPUT);
    pinMode(motArGena, OUTPUT);
    pinMode(pinA0, INPUT);
    pinMode(pinA1, INPUT);
    // Initialisation du port série
    Serial.begin(9600);
}
 void loop() {
  int a0 = analogRead(pinA0);
  int a1 = analogRead(pinA1);
   state = map(a1, 0, 1023, -255, 255);
   state2 = map(a0, 0, 1023, -255, 255);
   Serial.print("x = ");
   Serial.println(a0);
     
     
     
  
   // if (Serial.available() > 0)
    {
      // Lecture de l'entier passé au port série
      //state = Serial.parseInt();

      //
      // Sens du mouvement
      //
      
      if ((state > 0) && (state > state2)) // avant
      {
        if(motAvDactive == true)
        {
          digitalWrite(motAvDPin1, HIGH); 
          digitalWrite(motAvDPin2, LOW);
        }

        Serial.print("Avant ");
        Serial.println(state);
        if(motArDactive == true)
        {
            digitalWrite(motArDPin1, HIGH); 
            digitalWrite(motArDPin2, LOW);
        }

        if(motAvGactive == true)
        {
          digitalWrite(motAvGPin1, HIGH); 
          digitalWrite(motAvGPin2, LOW);
        }
        if(motArGactive == true)
        {
          digitalWrite(motArGPin1, HIGH); 
          digitalWrite(motArGPin2, LOW);
        }
      }
      else if ((state < 0) && (state < state2) )// arrière
      {

        if(motArDactive == true)
        {
          digitalWrite(motArDPin1, LOW); 
          digitalWrite(motArDPin2, HIGH);  
        }
        if(motAvDactive == true)
        {
          digitalWrite(motAvDPin1, LOW); 
          digitalWrite(motAvDPin2, HIGH);
        }
        if(motAvGactive == true)
        {
        digitalWrite(motAvGPin1, LOW); 
        digitalWrite(motAvGPin2, HIGH);
        }
        if(motArGactive == true)
        {
          digitalWrite(motArGPin1, LOW); 
          digitalWrite(motArGPin2, HIGH);
        }
        Serial.print("Arriere ");
        Serial.println(state);
      }
      else if(state2 < 0)
      {
      if(motAvGactive == true)
        {
        digitalWrite(motAvGPin1, LOW); 
        digitalWrite(motAvGPin2, HIGH);
        }
        if(motArGactive == true)
        {
          digitalWrite(motArGPin1, HIGH); 
          digitalWrite(motArGPin2, LOW);
        }
      }
      else if(state2 > 0)
      {
        if(motAvGactive == true)
        {
          digitalWrite(motAvGPin1, HIGH); 
          digitalWrite(motAvGPin2, LOW);
        }
        if(motArGactive == true)
        {
          digitalWrite(motArGPin1, LOW); 
          digitalWrite(motArGPin2, HIGH);
        }

      }

      else // Stop (freinage)
      {

        if(motArDactive == true)
        {
          digitalWrite(motArDPin1, HIGH); 
          digitalWrite(motArDPin2, HIGH);
        }
        if(motAvDactive == true)
        {
          digitalWrite(motAvDPin1, HIGH); 
          digitalWrite(motAvDPin2, HIGH);
        }
        Serial.println("Stop");
      }

      //
      // Vitesse du mouvement
      //
      if(motAvDactive == true)
      {
        analogWrite(motAvDena, abs(state));
      }
      if(motArDactive == true)
      {
        analogWrite(motArDena, abs(state));
      }
      if(motAvGactive == true)
      {
      analogWrite(motAvGena, abs(state));
      }
      if(motArGactive == true)
      {
      analogWrite(motArGena, abs(state));
      }
    }
    delay(100);

    
}