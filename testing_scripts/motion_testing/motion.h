// Command
#define XDIR  1
#define YDIR  -1
#define XAMT  0.1 * XDIR
#define YAMT  0.1 * YDIR
#define JOGAMT  0.2
#define MOTOR1 "X"
#define MOTOR2 "Y"
#define MOTOR3 "Z"
#define XMOTOR 1 
#define YMOTOR 0 // RMB TO CHANGE FOR ACTUAL TESTING
#define MOVE_JOG "$J=G91"
#define MOVE_RES "G91"
#define FEED_RATE "F1500"

#define XMAXRATESETUP "$110=1500.000"
#define YMAXRATESETUP "$111=600.000"
#define ZMAXRATESETUP "$112=700.000"


#ifndef motionClass
#define motionClass_h
#include "Arduino.h"
class motionManager {
  public:
    // Higher abstractions for arm belt
    void jogArmBeltLeft();
    void jogArmBeltRight();
    void moveArmBeltLeft(int amt);
    void moveArmBeltRight(int amt);

    // Higher abstractions for slider belt
    void jogSliderBeltDown();
    void jogSliderBeltUp();
    void moveSliderBeltDown(int amt);
    void moveSliderBeltUp(int amt);

  

    // Lower abstractions
    void motorJog(int motor_num, double dir);
    void motorMove(int motor_num, double amount);
    void motionStop();
    void motionSetup();
    void grbl_update();

  private:
    String motors[3] = {MOTOR1, MOTOR2, MOTOR3};
};
#endif
