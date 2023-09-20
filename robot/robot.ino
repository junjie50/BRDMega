// Command
const String MOVE_RES = "$J=G91";
const String FEED_RATE = "F1000";

// Sensor
const int SENSOR1 = 30; // Sensor at the start
const int SENSOR2 = 31;
const int SENSOR3 = 32;
const int SENSOR4 = 33; // Sensor at the start
const int SENSOR5 = 34;
const int SENSOR6 = 35;

// Motors
const char MOTOR1 = 'X';
const char MOTOR2 = 'Y';
const int X_DIR = 1;
const int Y_DIR = 1;
const double X_AMT = 0.1 * X_DIR;


// Logical Flags
bool idleState = true;
bool entry1 = false;
bool exit1 = true;
bool entry2 = false;
bool exit2 = true;
bool entry3 = false;
bool exit3 = true;


byte x;
char grbl_out;
String cmd;

// Function declaration.
void grbl_update();
void motion_stop();
void FSM();
void x_jog(double amount);
void motor_jog(char motor, double amount);
void test();
void idle();
void session();
void reset_first_tray();
void rack1_fsm();
void rack2_fsm();

void setup() {
  // put your setup code here, to run once:
  // initialize both serial ports:

  // Serial 0 talks to the computer
  Serial.begin(115200);

  // Serial 1 talks with the blackbox x32
  Serial1.begin(115200);


  //
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
}

void loop() {
  FSM();
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
  //
}


void reset_first_tray() {
  entry1 = false;
  exit1 = true;
}


//##########################MOTION CONTROL##########################//
void x_jog(double amount) {
  motor_jog(MOTOR1, amount);
}

void motor_jog(char motor, double amount) {
  String ops = MOVE_RES + String(motor);
  String sign = amount < 0?"-":"+";
  ops += (sign + String(abs(amount), 2) + FEED_RATE + "\n");
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
