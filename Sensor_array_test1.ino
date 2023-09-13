// Arduino IR Sensor array Code

int S1 = 30;
int S2 = 31;
int S3 = 32;
int S4 = 40;
int S5 = 41;
int S6 = 50;
int S7 = 51;

int LED = 13;


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Serial Working");

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  pinMode(S6, INPUT);
  pinMode(S7, INPUT);

  pinMode(LED, OUTPUT);  // LED Pin Output
}


void loop() {

  // read from port 0, print, send to port 1:
  if (Serial.available()) {
    String pcMSG = Serial.readString();
    Serial.println(pcMSG);
    Serial1.print(pcMSG);
  }

  // read from port 1, send to port 2 :
  if (Serial1.available()) {
    //int grblMSG = Serial1.read();
    String grblMSG = Serial1.readString();
    Serial.println(grblMSG);
  }

  bool S1state = digitalRead(S1);
  bool S2state = digitalRead(S2);
  bool S3state = digitalRead(S3);
  bool S4state = digitalRead(S4);
  bool S5state = digitalRead(S5);
  bool S6state = digitalRead(S6);
  bool S7state = digitalRead(S7);

  if (S1state == 0) {
    digitalWrite(LED, HIGH);
    Serial.println("1");
  } else {
    digitalWrite(LED, LOW);
  }

  if (S2state == 0) {
    //digitalWrite(LED, HIGH);
    //Serial.println("2");
  } else {
    digitalWrite(LED, LOW);
  }

  if (S3state == 0) {
    //digitalWrite(LED, HIGH);
    //Serial.println("3");
  } else {
    digitalWrite(LED, LOW);
  }

  if (S4state == 0) {
    digitalWrite(LED, HIGH);
    Serial.println("4");
  } else {
    digitalWrite(LED, LOW);
  }

  if (S5state == 0) {
    digitalWrite(LED, HIGH);
    Serial.println("5");
  } else {
    digitalWrite(LED, LOW);
  }

  if (S6state == 0) {
    digitalWrite(LED, HIGH);
    Serial.println("6");
  } else {
    digitalWrite(LED, LOW);
  }

  if (S7state == 0) {
    digitalWrite(LED, HIGH);
    Serial.println("7");
  } else {
    digitalWrite(LED, LOW);
  }
}
