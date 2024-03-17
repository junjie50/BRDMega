#include "Arduino.h"
#include "motion.h"

//Higher level abstractions for arm belt only.
void motionManager::jogRackOnlyBeltLeft() {
  jogRackBeltLeft();
  delay(4);
}

void motionManager::jogRackOnlyBeltRight() {
  jogRackBeltRight();
  delay(4);
}

void motionManager::moveRackOnlyBeltLeft(int amt) {
  moveRackBeltLeft(amt);
}

void motionManager::moveRackOnlyBeltRight(int amt) {
  moveRackBeltRight(amt);
}

//Higher level abstractions for arm belt only.
void motionManager::jogArmOnlyBeltLeft() {
  jogArmBeltLeft();
  delay(5);
}

void motionManager::jogArmOnlyBeltRight() {
  jogArmBeltRight();
  delay(5);
}

void motionManager::moveArmOnlyBeltLeft(int amt) {
  moveArmBeltLeft(amt);
}

void motionManager::moveArmOnlyBeltRight(int amt) {
  moveArmBeltRight(amt);
}


//Higher level abstractions for rack and arm belt
void motionManager::jogRackArmBeltLeft() {
  twoMotorJog(XMOTOR, ZMOTOR, XDIR);
  delay(5);
}

void motionManager::jogRackArmBeltRight(){
  twoMotorJog(XMOTOR, ZMOTOR, -XDIR);
  delay(5);
}

void motionManager::moveRackArmBeltLeft(int amt){
  twoMotorMove(XMOTOR, ZMOTOR, XDIR*amt, -XDIR*amt);
}

void motionManager::moveRackArmBeltRight(int amt){
  twoMotorMove(XMOTOR, ZMOTOR, -XDIR*amt, XDIR*amt);
}

void motionManager::jogArmBeltLeft() {
  motorJog(XMOTOR, XDIR);
}

void motionManager::jogArmBeltRight() {
  motorJog(XMOTOR, -XDIR);
}

void motionManager::moveArmBeltLeft(int amt) {
  motorMove(XMOTOR, XDIR * amt);
}

void motionManager::moveArmBeltRight(int amt) {
  motorMove(XMOTOR, -XDIR * amt);
}

//Higher level abstractions for rack belt
void motionManager::jogRackBeltLeft() {
  motorJog(ZMOTOR, ZDIR);
}

void motionManager::jogRackBeltRight(){
  motorJog(ZMOTOR, -ZDIR);
}

void motionManager::moveRackBeltLeft(int amt){
  motorMove(ZMOTOR, ZDIR * amt);
}

void motionManager::moveRackBeltRight(int amt){
  motorMove(ZMOTOR, -ZDIR * amt);
}


// For slider
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

// Basic movements
void motionManager::twoMotorJog(int motor_num, int motor_num2, double dir) {
  double amount = dir * JOGAMT;
  String motor = motors[motor_num];
  String motor2 = motors[motor_num2];
  String ops = MOVE_JOG;
  String sign = amount < 0?"-":"+";
  String sign2 = amount < 0?"+":"-";
  ops += (motor + sign + String(abs(amount), 2));
  ops += (motor2 + sign2 + String(abs(amount), 2));
  ops += (FEED_RATE + String("\n"));
  Serial1.print(ops);
}

void motionManager::motorMove(int motor_num, double amount) {
  String motor = motors[motor_num];
  String ops = MOVE_RES + motor;
  String sign = amount < 0?"-":"+";
  ops += (sign + String(abs(amount), 2) + "\n");
  Serial1.print(ops);
}

// 
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

void motionManager::motionSetup(){
  Serial1.println(XMAXRATESETUP);
  Serial1.println(YMAXRATESETUP);
  Serial1.println(ZMAXRATESETUP);
}
