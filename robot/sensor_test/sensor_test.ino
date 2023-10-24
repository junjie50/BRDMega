void setup() {
  // Put your setup code here, to run once:
  // Serial 0 talks to the computer
  Serial.begin(115200);
  pinMode(30, INPUT);
}

void loop() {
  // Put your main code here, to run repeatedly:
  Serial.println(digitalRead(34));
}
