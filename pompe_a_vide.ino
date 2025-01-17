void setup() {
  // put your setup code here, to run once:
pinMode(9, OUTPUT); // pompe
pinMode(5, OUTPUT); // valve
pinMode(10, INPUT); //telecommande
}

void loop() {
  // put your main code here, to run repeatedly:
/*digitalWrite (9, LOW);
digitalWrite(5, LOW);
delay(5000);
digitalWrite (9, HIGH);
delay(20000);
digitalWrite (9, LOW);
digitalWrite(5, HIGH);
delay(1000);*/
int aspire=digitalRead(10);
if(aspire==HIGH){
  digitalWrite(9, HIGH);
  digitalWrite(5, LOW);
}else{
  digitalWrite(9, LOW);
digitalWrite(5, HIGH);
}
delay(100);
}
