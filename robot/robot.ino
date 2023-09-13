int x;
void setup() {
  // put your setup code here, to run once:
  // initialize both serial ports:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!Serial.available());
    x = Serial.readString().toInt();
    Serial.print(x + 1);
}
