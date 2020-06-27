/*
 * Author: Zeel P
 * May 21, 2019 
 * Project: Fire-Fighting Robot
 * Decription: Robot navigates through a maze to find a flame and puts it out
 */

#include <Servo.h>

Servo leftWheel, rightWheel, flameChecker;

boolean forward;
int distancePin, flamePin, fanPin, distanceThreshold, flameTreshold, flameValues[180] = {},flameAngle;

void setup() {
  leftWheel.attach(2);
  rightWheel.attach(3);
  flameChecker.attach(4);
  resetWheels();
  forward = true;
  distanceThreshold = 375;
  flameTreshold = 600;
  distancePin = A0;
  flamePin = A1;
  pinMode(distancePin, INPUT);
  pinMode(flamePin, INPUT);
  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, HIGH);
  Serial.begin(9600);

}

void loop() {
  if(findFlame()){
    int turn = flameAngle - 90;
    digitalWrite(fanPin, HIGH);
    if(turn < 0){
      turn *= -1;
      resetWheels();
      attachWheels();
      leftWheel.write(180);
      rightWheel.write(106);
      delay(turn * (1/1250));
    }else if(turn > 0){
      resetWheels();
      attachWheels();
      leftWheel.write(0);
      rightWheel.write(76);
      delay(turn * (1 / 1250));
    }
  }else {
  if(!checkDistance()){
    moveForward();
  }else{
    Serial.println("Found wall, turning left");
    turnLeft();
    if(!checkDistance()){
      moveForward();
    }else{
      Serial.println("Found wall, turning right");
      turnRight();
      turnRight();
      moveForward();
    } 
   }
  }
}

boolean checkDistance(){
  int distance = analogRead(distancePin);
  Serial.print("Distance");
  Serial.println(distance);
  return distance > distanceThreshold;
}

boolean findFlame(){
  boolean found = false;
  flameChecker.write(0);
  delay(1000);
  int biggestValue = flameTreshold;
  for(int i = 0; i < 180; i++){
    flameChecker.write(i);
    delay(5);
    int flameValue = analogRead(flamePin);
    if(flameValue >= biggestValue){
      biggestValue = flameValue;
      flameAngle = i;
      found = true;
      Serial.print("Found flame at value: ");
      Serial.print(biggestValue);
      Serial.print(" at angle: ");
      Serial.println(flameAngle);
    }
  }
  return found;
  
}

void moveForward(){
  Serial.println("Moving forward");
  for(int i = 0; i < 10; i++){
    if(!checkDistance()){
      leftWheel.write(0);
      rightWheel.write(106);
      delay(500);
    }else {
      resetWheels();
      return;
    }
  }
  resetWheels();
}

void resetWheels(){
  leftWheel.write(90);
  rightWheel.write(90);
  delay(1000);
  leftWheel.detach();
  rightWheel.detach();
}

void attachWheels(){
  leftWheel.attach(2);
  rightWheel.attach(3);
}

void turnRight(){
  resetWheels();
  attachWheels();
  leftWheel.write(0);
  rightWheel.write(76);
  delay(1250);
}

void turnLeft(){
  resetWheels();
  attachWheels();
  leftWheel.write(180);
  rightWheel.write(106);
  delay(1250);
}
