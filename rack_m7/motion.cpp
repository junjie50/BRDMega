#include "Arduino.h"
#include "motion.h"

// Higher abstraction
// Movement plus direction change
void motionManager::moveRobotBufferLeft(int amt){
  return;
}
void motionManager::moveRobotBufferRight(int amt){
  return;
}
void motionManager::jogRobotBufferLeft() {
  return;
}
void motionManager::jogRobotBufferRight() {
  return;
}
void motionManager::moveHumanBufferLeft(int amt){
  return;
}

void motionManager::moveHumanBufferRight(int amt){
  return;
}

void motionManager::collectOneItem(){
  return;
}
void motionManager::depositOneItem(){
  return;
}

// FUNCTIONS TO CALL WHEN MOVING THE RACK.
void motionManager::moveY(int amt) {
  motorMove(YMOTOR, YDIR * amt);
}

void motionManager::moveX(int amt) {
  motorMove(XMOTOR, XDIR * amt);
}

void motionManager::moveXY(int amtx, int amty) {
  twoMotorMove(XMOTOR, YMOTOR, XDIR*amtx, YDIR*amty);
}

void motionManager::moveZ(int amt) {
  motorMove(ZMOTOR, ZDIR*amt);
}


// Other functions.
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
  Serial1.print(ops);
}

void motionManager::twoMotorMove(int motor_num, int motor_num2, double amount1, double amount2) {
  String motor = motors[motor_num];
  String motor2 = motors[motor_num2];
  String ops = MOVE_RES;
  String sign = amount1 < 0?"-":"+";
  String sign2 = amount2 < 0?"+":"-";
  ops += (motor + sign + String(abs(amount1), 2));
  ops += (motor2 + sign2 + String(abs(amount2), 2));
  ops +=  "\n";
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