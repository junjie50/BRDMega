// Command
#define XDIR  1
#define YDIR  1
#define XAMT  0.1 * XDIR
#define YAMT  0.1 * YDIR
#define JOGAMT  0.1
#define MOTOR1 'X'
#define MOTOR2  'Y'
#define MOTOR3 'Z'
#define XMOTOR 0 
#define YMOTOR 0 // RMB TO CHANGE FOR ACTUAL TESTING
#define MOVE_JOG "$J=G91"
#define MOVE_RES "G91"
#define FEED_RATE "F1000"


#ifndef motionClass
#define motionClass_h
#include "Arduino.h"
class motionManager {
  public:
    // Higher abstractions
    void jogArmBeltLeft();
    void jogSliderBeltDown();

    // Lower abstractions
    void motorJog(int motor_num, double dir);
    void motorMove(int motor_num, double amount);
    void motionStop();
    void grbl_update();

  private:
    char motors[3] = {MOTOR1, MOTOR2, MOTOR3};
};

#endif
