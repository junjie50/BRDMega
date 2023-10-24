#include "Arduino.h"
#include "motion.h"

//Higher level abstractions
void motionManager::jogArmBeltLeft() {
  motorJog(XMOTOR, XDIR);
  delay(5);
}

void motionManager::jogArmBeltRight() {
  motorJog(XMOTOR, -XDIR);
  delay(5);
}

void motionManager::moveArmBeltLeft(int amt) {
  motorMove(XMOTOR, XDIR * amt);
}

void motionManager::moveArmBeltRight(int amt) {
  motorMove(XMOTOR, -XDIR * amt);
}

void motionManager::jogSliderBeltDown() {
  motorJog(YMOTOR, -YDIR);
  delay(5);
}

void motionManager::jogSliderBeltUp() {
  motorJog(YMOTOR, YDIR);
  delay(5);
}

void motionManager::moveSliderBeltDown(int amt) {
  motorMove(YMOTOR, -YDIR * amt);
}

void motionManager::moveSliderBeltUp(int amt) {
  motorMove(YMOTOR, YDIR * amt);
}


// Basic movements
void motionManager::motorJog(int motor_num, double dir) {
  double amount = dir * JOGAMT;
  String motor = motors[motor_num];
  String ops = MOVE_JOG + motor;
  String sign = amount < 0?"-":"+";
  ops += (sign + String(abs(amount), 2) + FEED_RATE + "\n");
  Serial1.print(ops);
}

void motionManager::motorMove(int motor_num, double amount) {
  String motor = motors[motor_num];
  String ops = MOVE_RES + motor;
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
