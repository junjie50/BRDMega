// Command
#define XDIR  1
#define YDIR  -1
#define ZDIR -1
#define JOGAMT  1
#define BOTHJOGAMT 0.4
#define MOTOR1 "X"
#define MOTOR2 "Y"
#define MOTOR3 "Z"
#define XMOTOR 1 
#define YMOTOR 0 // RMB TO CHANGE FOR ACTUAL TESTING
#define ZMOTOR 2 // RMB TO CHANGE FOR ACTUAL TESTING
#define MOVE_JOG "$J=G91"
#define MOVE_RES "G91"
#define FEED_RATE "F1500"

// Default settings command
#define XMAXRATESETUP "$110=1500.000"
#define YMAXRATESETUP "$111=600.000"
#define ZMAXRATESETUP "$112=600.000"


#ifndef motionClass
#define motionClass_h
#include "Arduino.h"
class motionManager {
  public:

    void jogArmOnlyBeltLeft();
    void jogArmOnlyBeltRight();
    void moveArmOnlyBeltLeft(int amt);
    void moveArmOnlyBeltRight(int amt);

    void jogRackOnlyBeltLeft();
    void jogRackOnlyBeltRight();
    void moveRackOnlyBeltLeft(int amt);
    void moveRackOnlyBeltRight(int amt);

    //Higher abstraction for rackarm belt
    void jogRackArmBeltLeft();
    void jogRackArmBeltRight();
    void moveRackArmBeltLeft(int amt);
    void moveRackArmBeltRight(int amt);

    //Higher abstraction for arm belt
    void jogArmBeltLeft();
    void jogArmBeltRight();
    void moveArmBeltLeft(int amt);
    void moveArmBeltRight(int amt);

    //Higher abstraction for rack belt
    void jogRackBeltLeft();
    void jogRackBeltRight();
    void moveRackBeltLeft(int amt);
    void moveRackBeltRight(int amt);

    // Higher abstractions for slider belt
    void jogSliderBeltDown();
    void jogSliderBeltUp();
    void moveSliderBeltDown(int amt);
    void moveSliderBeltUp(int amt);


    // Lower abstractions
    void motorJog(int motor_num, double dir);
    void twoMotorJog(int motor_num, int motor_num2, double dir);
    void motorMove(int motor_num, double amount);
    void twoMotorMove(int motor_num, int motor_num2, double amount1, double amount2);
    void motionStop();
    void motionSetup();
    void grbl_update();

  private:
    String motors[3] = {MOTOR1, MOTOR2, MOTOR3};
};
#endif
