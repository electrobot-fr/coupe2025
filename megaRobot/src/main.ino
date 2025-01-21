#include "Arduino.h"
#include <SerialTransfer.h>
#include "Wire.h"

//Ports de commande du moteur B
int motArGPin1 = 8;
int motArGPin2 = 9;
int motArGena = 13;
bool motArGactive = true; //n°4

int motArDPin1 = 12;
int motArDPin2 = 11;
int motArDena= 10;
bool motArDactive = true; //n°2

int motAvDPin1 = 7;
int motAvDPin2 = 6;
int motAvDena = 5;
bool motAvDactive = true; //n°1

int motAvGPin1 = 3;
int motAvGPin2 = 2;
int motAvGena = 4;
bool motAvGactive = true; //n°3

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
    if(motArGactive == true)
    {
      pinMode(motArGPin1, OUTPUT);
      pinMode(motArGPin2, OUTPUT);
      pinMode(motArGena, OUTPUT);
   }
    
    
    pinMode(motAvDPin1, OUTPUT);
    pinMode(motAvDPin2, OUTPUT);
    pinMode(motAvDena, OUTPUT);
    pinMode(motAvGPin1, OUTPUT);
    pinMode(motAvGPin2, OUTPUT);
    pinMode(motAvGena, OUTPUT);
    pinMode(pinA0, INPUT);
    pinMode(pinA1, INPUT);
    // Initialisation du port série
    Serial.begin(9600);
}
 void loop() {
  int a0 = analogRead(pinA0);
  int a1 = analogRead(pinA1);
   state = map(a1, 0, 1023, -250, 250);
   state2 = map(a0, 0, 1023, -250, 250);
   Serial.print("state2 = ");
   Serial.println(state2);
     
   // if (Serial.available() > 0)
    {
      // Lecture de l'entier passé au port série
      //state = Serial.parseInt();

      //
      // Sens du mouvement
      //
      
      if ((state > 0) && (abs(state)>abs(state2)))// avant
      {
      

        if(motArGactive == true)
        {
          digitalWrite(motArGPin1, HIGH); 
          digitalWrite(motArGPin2, LOW);
        }

        Serial.print("Avant ");
        Serial.println(state);
        if(motArDactive == true)
        {
            digitalWrite(motArDPin1, HIGH); 
            digitalWrite(motArDPin2, LOW);
        }

        if(motAvDactive == true)
        {
          digitalWrite(motAvDPin1, HIGH); 
          digitalWrite(motAvDPin2, LOW);
        }
        if(motAvGactive == true)
        {
          digitalWrite(motAvGPin1, HIGH); 
          digitalWrite(motAvGPin2, LOW);
        }
      }
      else if ((state < 0) && (abs(state)>abs(state2)))// arrière
      {
       

        if(motArDactive == true)
        {
          digitalWrite(motArDPin1, LOW); 
          digitalWrite(motArDPin2, HIGH);  
        }
        if(motArGactive == true)
        {
          digitalWrite(motArGPin1, LOW); 
          digitalWrite(motArGPin2, HIGH);
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
        Serial.print("Arriere ");
        Serial.println(state);
      }
      else if ((state2 < 0) && (abs(state2)>abs(state)))
      {
         Serial.println("Droite");

        if(motAvDactive == true)
        {
          digitalWrite(motAvDPin1, LOW); 
          digitalWrite(motAvDPin2, HIGH);
        }
        if(motAvGactive == true)
        {
          digitalWrite(motAvGPin1, HIGH); 
          digitalWrite(motAvGPin2, LOW);
        }
        if(motArDactive == true)
        {
          digitalWrite(motArDPin1, HIGH); 
          digitalWrite(motArDPin2, LOW);
        }
        if(motArGactive == true)
        {
          digitalWrite(motArGPin1, LOW); 
          digitalWrite(motArGPin2, HIGH);
        }
      }
      else if ((state2 > 0) && (abs(state2)>abs(state)))
      {
        Serial.println("Gauche");
        if(motAvDactive == true)
        {
          digitalWrite(motAvDPin1, HIGH); 
          digitalWrite(motAvDPin2, LOW);
        }
        if(motAvGactive == true)
        {
          digitalWrite(motAvGPin1, LOW); 
          digitalWrite(motAvGPin2, HIGH);
        }
        if(motArDactive == true)
        {
          digitalWrite(motArDPin1, LOW); 
          digitalWrite(motArDPin2, HIGH);
        }
        if(motArGactive == true)
        {
          digitalWrite(motArGPin1, HIGH); 
          digitalWrite(motArGPin2, LOW);
        }
      }
      else // Stop (freinage)
      {

        if(motArDactive == true)
        {
          digitalWrite(motArDPin1, HIGH); 
          digitalWrite(motArDPin2, HIGH);
        }
        if(motArGactive == true)
        {
          digitalWrite(motArGPin1, HIGH); 
          digitalWrite(motArGPin2, HIGH);
        }
        if(motAvDactive == true)
        {
          digitalWrite(motAvDPin1, HIGH);
          digitalWrite(motAvDPin2, HIGH);
        }
        if(motAvGactive == true)
        {
          digitalWrite(motAvGPin1, HIGH);
          digitalWrite(motAvGPin2, HIGH);
        }

        Serial.println("Stop");
      }

      //
      // Vitesse du mouvement
      //
      if (abs(state2)>abs(state))
      {
       state = state2;
      }
      if(motArGactive == true)
      {
        analogWrite(motArGena, abs(state));
      }
      if(motArDactive == true)
      {
        analogWrite(motArDena, abs(state));
      }
      if(motAvDactive == true)
      {
      analogWrite(motAvDena, abs(state));
      }
      if(motAvGactive == true)
      {
      analogWrite(motAvGena, abs(state));
      }
    }
    delay(100);

    
}