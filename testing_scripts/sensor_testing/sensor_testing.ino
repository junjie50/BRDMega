#include "sensor.h"

sensorManager sensor;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  sensor.sensorSetUp();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(sensor.sensorActivated(0)) {
    Serial.println("30 activated");
  }
  if(sensor.sensorActivated(1)) {
    Serial.println("31 activated");
  }
  if(sensor.sensorActivated(2)) {
    Serial.println("32 activated");
  }
  if(sensor.sensorActivated(3)) {
    Serial.println("33 activated");
  }
  if(sensor.sensorActivated(4)) {
    Serial.println("34 activated");
  }
   if(sensor.sensorActivated(6)) {
    Serial.println("36 activated");
  }
  if(sensor.sensorActivated(7)) {
    Serial.println("37 activated");
  }
  delay(50);


  if(Serial1.available()){
    Serial.println(Serial1.readString());
  }
}
