#include "Arduino.h"
#include "motion.h"

//Higher level abstractions
void motionManager::jogArmBeltLeft() {
  motorJog(XMOTOR, XDIR);
}

void motionManager::jogSliderBeltDown() {
  motorJog(YMOTOR, YDIR);
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
