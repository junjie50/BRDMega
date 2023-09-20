// Command
const String MOVE_RES = "$J=G91";
const String FEED_RATE = "F1000";

// Sensor
const int SENSOR1 = 30; // Sensor at the start
const int SENSOR2 = 31;
const int SENSOR3 = 32;

// Motors
const char motor1 = 'X';
const char motor2 = 'Y';

// Distance
const int dist_tray_1 = 250;

// Logical Flags
bool ENTRY1 = false;
bool EXIT1 = false;


byte x;
char grbl_out;
String cmd;

// Function declaration.
void grbl_update();
void motion_stop();
void FSM();
void move_motor_res(char motor, int amount);

void setup() {
  // put your setup code here, to run once:
  // initialize both serial ports:

  // Serial 0 talks to the computer
  Serial.begin(115200);

  // Serial 1 talks with the blackbox x32
  Serial1.begin(115200);

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  pinMode(sensor3, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Currently testing the motor.

  // Testing the sensor part
  if(!digitalRead(sensor1)) {
    Serial.println("Sensor 1 detected, moving motor.");
    move_motor_res('X', -0.1);
  }

  if(!digitalRead(sensor2)) {
    Serial.println("Sensor 2 detected, stopping motor.");
    motion_stop();
  }
}

// Creating the logic for the machine.
void FSM() {
  
}

void move_motor_res(char motor, double amount) {
  String sign = amount < 0?"-":"+";
  String ops = MOVE_RES + String(motor);
  ops += (sign + String(abs(amount), 2) + FEED_RATE + "\n");
  Serial.println(ops);

  // Calling for the motor to move
  Serial1.print(ops);
}

void motion_stop() {
  String ops ="!\n";
  Serial.println(ops);
  // Calling for the motor to stop
  Serial1.write(133);
}

// Function to print update on Serial0.
void grbl_update() {
  if (Serial1.available()) {
    String grblMSG = Serial1.readString();
    Serial.println(grblMSG);
  }
}
