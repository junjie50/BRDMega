#include "Arduino.h"
#include "motion.h"


//Higher abstraction
// Movement plus direction change
void motionManager::moveY(int amt) {
  if (amt < 0) {
    moveSliderDown(-amt);
  }
  else{
    moveSliderUp(amt);
  }
}

void motionManager::moveX(int amt) {
  if (amt < 0) {
    moveSliderLeft(-amt * COLUMNWIDTH);
  }
  else{
    moveSliderRight(amt * COLUMNWIDTH);
  }
}

void motionManager::moveZ(int amt) {
  if (amt < 0) {
    moveCarBack(-amt);
  }
  else{
    moveCarFront(amt);
  }
}

void motionManager::moveCarFront(int amt){
  motorMove(ZMOTOR, ZDIR * amt);
}

void motionManager::moveCarBack(int amt){
  motorMove(ZMOTOR, -ZDIR * amt);
}

void motionManager::jogSliderRight() {
  motorJog(XMOTOR, XDIR);
  delay(5);
}

void motionManager::moveSliderRight(int amt) {
  motorMove(XMOTOR, XDIR * amt);
}

void motionManager::jogSliderLeft() {
  motorJog(XMOTOR, -XDIR);
  delay(5);
}

void motionManager::moveSliderLeft(int amt) {
  motorMove(XMOTOR, -XDIR * amt);
}

void motionManager::jogSliderUp() {
  motorJog(YMOTOR, YDIR);
  delay(5);
}

void motionManager::moveSliderUp(int amt) {
  motorMove(YMOTOR, YDIR * amt);
}

void motionManager::moveSliderDown(int amt) {
  motorMove(YMOTOR, -YDIR * amt);
}

// Basic movements
void motionManager::motorJog(int motor_num, double dir) {
  double amount = dir * JOGAMT;
  char motor = motors[motor_num];
  String ops = MOVE_JOG + String(motor);
  String sign = amount < 0?"-":"+";
  ops += (sign + String(abs(amount), 2) + FEED_RATE + "\n");
  Serial1.print(ops);
}

void motionManager::motorMove(int motor_num, double amount) {
  char motor = motors[motor_num];
  String ops = MOVE_RES + String(motor);
  String sign = amount < 0?"-":"+";
  ops += (sign + String(abs(amount), 2) + "\n");
  //Serial.println(ops);
  // Calling for the motor to move
  Serial1.print(ops);
}

// Stops everything.
void motionManager::motionStop() {
  String ops ="!\n";
  Serial1.write(133);
}

// Function to print update on Serial0.
void motionManager::grbl_update() {
  if (Serial1.available()) {
    String grblMSG = Serial1.readString();
    Serial.println(grblMSG);
  }
}