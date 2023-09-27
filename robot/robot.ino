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
bool idleState = true;
bool entry1 = false;
bool exit1 = true;
bool entry2 = false;
bool exit2 = true;
bool entry3 = false;
bool exit3 = true;

// Int side = 0 or 1 for y tray. this value indicates the bottom
int curr_tray = 0;
int start = 0;


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
void motor_res(char motor, double amount) ;
void test();
void idle();
void session();
void reset_first_tray();
void rack1_fsm();
void rack2_fsm();

void setup() {
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

void loop() {
  if(!systemReady) {
    reset();
  }
  else {
    Serial.println("System Ready");
  }
}

// Creating the logic for the machine.
void FSM() {
  if(idleState) {
    idle();
  }
  else {
    session();
  }
}

// Code flow for idle state to prevent balooned code.
// Idle state to check for detection of sensors/robot or from rack.
void idle() {
  if(!digitalRead(SENSOR1)) {
    entry1 = true;
    exit1 = false;
    idleState = false;
  }
  else{
    delay(100);
  }
  // Add in other sensor triggers
}

// Code flow for when object has been detected.
void session() {
  if(entry1) {
    Serial.println("In first tray");
    rack1_fsm();
  }
  else if (entry2) {
    Serial.println("In second tray");
    rack2_fsm();
  }
}


// rack1 FSM
void rack1_fsm() {
  if(!exit1) {
    x_jog(X_AMT);
    if(!digitalRead(SENSOR2)) {
      exit1 = true;
    }
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


// rack1 FSM Second tray
void rack2_fsm() {
  if (!exit2) {
    // Jogging item to end of sensor 4.
    x_jog(X_AMT);
    if(!digitalRead(SENSOR4)) {
      exit2 = true;
      y_move(Y_AMT);
    }
  }
  else {
    if(Serial1.available()) {
      
    }
  }
}


void reset_first_tray() {
  entry1 = false;
  exit1 = true;
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
