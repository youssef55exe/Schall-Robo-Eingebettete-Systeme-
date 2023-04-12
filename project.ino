#include <SoftwareSerial.h>
#include <stdio.h>

int motorR1 = 4;
int motorR2 = 5;
int enA=9;

int motorL1 = 10;
int motorL2 = 7;
int enB = 6;

int echoR = 2;
int triggerR = 8;
int echo = 11;
int trigger = 12;

int piepser = 3;
bool piepwert;

bool autofahren;

char Sensor;
char incoming_value;

unsigned int entfernung;
unsigned long dauer;

unsigned int entfernungR;
unsigned long dauerR;

unsigned long startMillis;
unsigned long aktuellerMillis;

void measureDist(){
  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(10);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);
  entfernung = (dauer / 2) * 0.03432;
}

void measureDistR(){
  digitalWrite(triggerR, LOW);
  delay(5);
  digitalWrite(triggerR, HIGH);
  delay(10);
  digitalWrite(triggerR, LOW);
  dauerR = pulseIn(echoR, HIGH);
  entfernungR = (dauerR / 2) * 0.03432;
}

void motorAus(){
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, LOW);
  analogWrite(enA,0);
  analogWrite(enB,0);
}

void rechts(){
  digitalWrite(motorL1, LOW);
  digitalWrite(motorL2, HIGH);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
  analogWrite(enA,78);
  analogWrite(enB,94);
}

void links(){
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, LOW);
  digitalWrite(motorR2, HIGH);
  analogWrite(enA,78);
  analogWrite(enB,94);
}

void motorAn(){
  digitalWrite(motorL1, HIGH);
  digitalWrite(motorL2, LOW);
  digitalWrite(motorR1, HIGH);
  digitalWrite(motorR2, LOW);
  analogWrite(enA,78);
  analogWrite(enB,94);
}

void autoPieps(){
  if(piepwert){ 
    measureDist();
    aktuellerMillis = millis();

    if (entfernung <= 21 && entfernung > 14) {
      if (aktuellerMillis - startMillis >= entfernung * 23) {
        tone(piepser, 200);
        delay(10);
        noTone(piepser);
        startMillis = aktuellerMillis;
        return;
      }
   }
    if (entfernung <= 14 && entfernung > 7) {
      if (aktuellerMillis - startMillis >= entfernung * 23) {
        Serial.println("Hallo 2");
        tone(piepser, 200);
        delay(10);
        noTone(piepser);
        startMillis = aktuellerMillis;
        return;
      }
   }
    if (entfernung <= 7) {
      if (aktuellerMillis - startMillis >= entfernung * 23) {
        Serial.println("Hallo 3");
        tone(piepser, 200);
        delay(10);
        noTone(piepser);
        startMillis = aktuellerMillis;
        return;
      }
    }
  }

  
}
void autonom2() { 
  measureDist();
  measureDistR();

  if (entfernung >= 15 && entfernungR > 40) {
    //nach rechts drehen und in die seiten gasse fahren.
    delay(450); //------------------------->
    autoPieps();
    motorAus();
    delay(1000);
    autoPieps();
    rechts();
    delay(590); //------------------------->
    autoPieps();
    motorAus();
    delay(1000);
    autoPieps();
    motorAn();
    delay(500); //------------------------->
    autoPieps();

  } else if (entfernung < 15) {
    motorAus();
    autoPieps();
    delay(500);
    measureDistR();
    autoPieps();

    if (entfernungR > 40) {
      rechts();
      autoPieps();
      delay(525);   //------------------------->
      motorAus();
      autoPieps();
      delay(1000);
      autoPieps();
    } else {
      links();
      autoPieps();
      delay(588);  //------------------------->
      autoPieps();
      motorAus();
      delay(500);
      autoPieps();
      measureDist();
      if(entfernung  > 50){
        
      } else {
        links();
        autoPieps();
        delay(550);   //------------------------->
        motorAus();
        autoPieps();
      }
      autoPieps();
      delay(1000);
      autoPieps();
    }
  }
}


void solveLab() {  

  measureDist();
  //WENN VORNE HINDERNIS MACH MOTOR AUS UND DREHT SICH NACH LINKS
  if (entfernung < 12) {
    motorAus();
    autoPieps();
    delay(1000);
    autoPieps();
    links();
    delay(595);
    motorAus();
    autoPieps();
    delay(1000);
    autoPieps();

    measureDist();
    // WENN AUCH LINKS DIREKT EIN HINDERNIS IST DANN SOLL DER SICH EINMAL UM 180° DREHEN
    if (entfernung < 12) {
      rechts();
      autoPieps();
      delay(1060);
      autoPieps();
      motorAus();
      delay(1000);
      autoPieps();

      measureDist();
      // WENN AUCH DA WIEDER EIN HINDERNIS IST DREHT ES NOCHMAL UM 90° NACH RECHTS UND FÄHRT ZURÜCK.
      if (entfernung < 12) {
        rechts();
        autoPieps();
        delay(635);
        autoPieps();
        motorAus();
        delay(1000);
        autoPieps();
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);
  pinMode(piepser, OUTPUT);
  pinMode(echoR, INPUT);
  pinMode(triggerR, OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(enB,OUTPUT);

  startMillis = millis();
}


void loop() {






  autoPieps();

  if(autofahren){
    motorAn();
    if(Sensor == 1){
      solveLab();
    }
    if(Sensor == 3){
      autonom2();
    }
  }
  
  if(Serial.available() > 0) {
    incoming_value = Serial.read();

    if (incoming_value == '1') {
      Sensor = 1;
    }
    if (incoming_value == '3') {
      Sensor = 3;
    }

    if (incoming_value == 'P') {
      piepwert = true;
    }

    if (incoming_value == 'p') {
      piepwert = false;
    }

    if (incoming_value == 'F') {
      motorAn();
    }

    if (incoming_value == 's') {
      motorAus();
    }
    if (incoming_value == 'B') {
      digitalWrite(motorL1, LOW);
      digitalWrite(motorL2, HIGH);
      digitalWrite(motorR1, LOW);
      digitalWrite(motorR2, HIGH);
      analogWrite(enA,78);
      analogWrite(enB,94);
    }

    if (incoming_value == 'L') {
      links();
    }

    if (incoming_value == 'R') {
      rechts();
    }

    if (incoming_value == 'A') {
      autofahren = true;
    }

    if (incoming_value == 'S') {
      autofahren = false;
      motorAus();
      Sensor = 0;
    }
  }
  
}