
/*
 '$J=' to indicate the jog
*/

// Command
#define XDIR  1
#define YDIR  1
#define ZDIR -1
#define XAMT  0.1 * XDIR
#define YAMT  0.1 * YDIR
#define ZAMT 0.1 * YDIR
#define JOGAMT 0.2
#define MOTOR1 "X"
#define MOTOR2 "Y"
#define MOTOR3 "Z"
#define XMOTOR 0 
#define YMOTOR 1 // RMB TO CHANGE FOR ACTUAL TESTING
#define ZMOTOR 2
#define MOVE_JOG "$J=G91"
#define MOVE_RES "G91"
#define FEED_RATE "F200"

// CONSTANTS FOR MOVEMENT
#define COLUMNWIDTH 30


#ifndef motionClass
#define motionClass_h
#include "Arduino.h"
class motionManager {
  public:
    // Higher utility functions
    void moveY(int amt);
    void moveX(int amt);
    void moveXY(int amtx, int amty);
    void moveZ(int amt);
    
    void moveCarFront(int amt);
    void moveCarBack(int amt);
    void jogSliderRight();
    void moveSliderRight(int amt);
    void jogSliderLeft();
    void moveSliderLeft(int amt);
    void moveSliderUp(int amt);
    void jogSliderUp();
    void moveSliderDown(int amt);

    void moveRobotBufferLeft(int amt);
    void moveRobotBufferRight(int amt);
    void jogRobotBufferLeft();
    void jogRobotBufferRight();
    void moveHumanBufferLeft(int amt);
    void moveHumanBufferRight(int amt);

    void collectOneItem(); // Function for car to go infront, take one item and return.
    void depositOneItem(); // Function for car to go infront, deposit one item and return.
    
    // Lower abstractions
    void motorJog(int motor_num, double dir);
    void motorMove(int motor_num, double amount);
    void twoMotorMove(int motor_num, int motor_num2, double amount1, double amount2);
    void motionStop();
    void grbl_update();

  private:
    String motors[3] = {MOTOR1, MOTOR2, MOTOR3};
};
#endif