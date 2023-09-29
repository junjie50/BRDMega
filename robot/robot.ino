// Command
const String MOVE_JOG = "$J=G91";
const String MOVE_RES = "G91";
const String FEED_RATE = "F1000";

// Sensor
const int SENSOR1 = 30; // Sensor at the start
const int SENSOR2 = 31;
const int SENSOR3 = 32;
const int SENSOR4 = 33; // Sensor at the start
const int SENSOR5 = 34;
const int SENSOR6 = 35;

// Touch sensor
const int TOUCH1 = 36;
const int TOUCH2 = 37;

// Motors
const char MOTOR1 = 'X';
const char MOTOR2 = 'Y';
const char MOTOR3 = 'Z';
const int X_DIR = 1;
const int Y_DIR = 1;
const double X_AMT = 0.1 * X_DIR;
const double Y_AMT = 0.1 * Y_DIR;

// Logical Flags
int armMode = 1; // 1 means taking item from arm 0 means taking item from rack
bool idleState = true;
bool entry1 = false;
bool exit1 = true;
bool entry2 = false;
bool pause2 = false;
bool exit2 = true;
bool entry3 = false;
bool exit3 = true;

// Reset logical flags
bool trayYReset = false;
bool systemReady = false;

// Int side = 0 or 1 for y tray
int currTrayY = 0;
int startTrayY = 0;


byte x;
char grbl_out;
String cmd;

// Function declaration.
void sensors_serial_setup();
void reset();
void grbl_update();
void motion_stop();
void FSM();
void x_jog(double amount);
void y_jog(double amount);
void motor_jog(char motor, double amount);
void motor_res(char motor, double amount);
void trayYToBottom();
void YBottomCheck();
void trayYToTop();
void yTopCheck();
void test();
void idle();
void sessionArm();
void reset_first_tray();

void reset_second_tray();
void rack1_fsm();
void rack2_fsm();
void rack3_fsm();

void setup1() {
  sensors_serial_setup();
}

//#######################SETUPS#########################//
void sensors_serial_setup() {
  // Serial 0 talks to the computer
  Serial.begin(115200);

  // Serial 1 talks with the blackbox x32
  Serial1.begin(115200);


  //
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);

  pinMode(TOUCH1, INPUT);
}



//########################FSM#########################//
// Reset the Position of the tray so that we can keep track of it.
void reset() {
  if(!trayYReset) {
    x_jog(Y_AMT);
    if(!digitalRead(TOUCH1)) {
      trayYReset = true;
      motion_stop();
    }

    if(trayYReset) {
      systemReady = true;
    }
  }
}

void loo1p() {
  if(!systemReady) {
    // RESET POSITION 
    reset();
  }
  else {
    FSM();
  }
}

// Creating the logic for the machine.
void FSM() {
  if(idleState) {
    idle();
  }
  else if(armMode) {
    sessionArm();
  }
}

// Code flow for idle state to prevent balooned code.
// Idle state to check for detection of sensors/robot or from rack.
void idle() {
  if(!digitalRead(SENSOR1)) {
    entry1 = true;
    exit1 = false;
    idleState = false;
    armMode = 1;
  }
  else{
    delay(100);
  }
  // Add in other sensor triggers
}

// Code flow for when object has been detected.
void sessionArm() {
  if(entry1) {
    Serial.println("In first tray");
    rack1_fsm();
  }
  if (entry2){
    Serial.println("In second tray");
    rack2_fsm();
  }
  if(entry3){
    Serial.println("In third tray");
    rack3_fsm();
  }
}

// FSM
void rack1_fsm() {
  if(armMode) {
    // Mode for collecting from the arm.
    if(!exit1) {
      x_jog(X_AMT);
      trayYToBottom(); // Get the tray to the bottom too.
      if(!digitalRead(SENSOR2)) {
        exit1 = true;
      }
    }
    else if (trayYAvailable) {
      trayYToBottom();
    }
    else{
      if(!entry2) {
        x_jog(X_AMT);
        if(!digitalRead(SENSOR3)) {
          entry2 = true;
          reset_first_tray();
        }
      }
    }
  }
}

// FSM Second tray
void rack2_fsm() {
  if(armMode) {
    if(!pause2) {
      // Jogging item to end of sensor 4.
      x_jog(X_AMT);
      trayYToTop();
      if(!digitalRead(SENSOR4)) {
        pause2 = true;
      }
    }
    else if (currTrayY == startTrayY) {
      // Wait for tray to reach the top.
      trayYToTop();
    }
    else{
      // Wait for item to hit tray 3.
      x_jog(-X_AMT); // return to opening of tray
      if(!digitalRead(SENSOR5)) {
        reset_second_tray();
        entry3 = true;
      }
    }
  }
}

void rack3_fsm() {
  if(armMode) {
    if(digitalRead(SENSOR5)) {
      x_jog(-X_AMT);
    }
  }
}

//Checking whether tray Y is engaged in action. 
bool trayYAvailable(){
  return !pause2;
}

void trayYToBottom() {
  if(trayYAvailable() && currTrayY != startTrayY){
    y_jog(Y_AMT);
    YBottomCheck();
  }
}

void YBottomCheck() {
  if(!digitalRead(TOUCH1)) {
    currTrayY = 0;
    motion_stop();
  }
}

void trayYToTop() {
  if(currTrayY == startTrayY) {
    y_jog(-Y_AMT);
    yTopCheck();
  }
}

void yTopCheck() {
  if(!digitalRead(TOUCH2)) {
    currTrayY = 1;
    motion_stop();
  }
}


void reset_first_tray() {
  entry1 = false;
  exit1 = true;
}

void reset_second_tray() {
  entry2 = false;
  exit2 = true;
  pause2 = false;
}


//##########################MOTION CONTROL##########################//
void x_jog(double amount) {
  motor_jog(MOTOR1, amount);
}

void y_jog(double amount) {
  motor_jog(MOTOR3, amount);
}

void y_move(double amount) {
  motor_res(MOTOR3, amount);
}

void motor_jog(char motor, double amount) {
  String ops = MOVE_JOG + String(motor);
  String sign = amount < 0?"-":"+";
  ops += (sign + String(abs(amount), 2) + FEED_RATE + "\n");
  //Serial.println(ops);
  // Calling for the motor to move
  Serial1.print(ops);
}

void motor_res(char motor, double amount) {
  String ops = MOVE_RES + String(motor);
  String sign = amount < 0?"-":"+";
  ops += (sign + String(abs(amount), 2) + "\n");
  //Serial.println(ops);
  // Calling for the motor to move
  Serial1.print(ops);
}

void motion_stop() {
  String ops ="!\n";
  Serial.println(ops);
  Serial1.write(133);
}

// Function to print update on Serial0.
void grbl_update() {
  if (Serial1.available()) {
    String grblMSG = Serial1.readString();
    Serial.println(grblMSG);
  }
}



//##########################TESTING##########################//
void test() {
  // put your main code here, to run repeatedly:
  // Currently testing the motor.

  // Testing the sensor part
  if(!digitalRead(SENSOR1)) {
    Serial.println("Sensor 1 detected, moving motor.");
    motor_jog('X', -0.1);
  }

  if(!digitalRead(SENSOR2)) {
    Serial.println("Sensor 2 detected, stopping motor.");
    motion_stop();
  }
}
