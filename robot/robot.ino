byte x;


void setup() {
  // put your setup code here, to run once:
  // initialize both serial ports:
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.print("Serial0 connected\n");
  Serial.print("Serial 1 connected\n");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    x = Serial.read();
    Serial.print(x);
    switch(x) {
      case 116:
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        break;
    }
  }
}
