#include "motion.h"
#include "sensor.h"

motionManager motion;
sensorManager sensors;

bool done = false;
// Jog slider up to touch sensor, set the position control variable.
// While moving change the currTray to -1 to indicate invalid position.
void jogSliderUpUntilTouch() {
  if(sensors.sensorActivated(7)) {
    motion.motionStop();
    motion.moveSliderBeltDown(5);
    delay(1000);
    done = true;
  }
  else {
    motion.jogSliderBeltUp();
  }
}

void jogSliderDownUntilTouch() {
  if(sensors.sensorActivated(6)) {
    motion.motionStop();
    motion.moveSliderBeltUp(15);
    delay(1000);
    done = true;
  }
  else {
    motion.jogSliderBeltDown();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  delay(1000);
  sensors.sensorSetUp();
  motion.motionSetup();
  delay(1000);
}

void loop() {
  //put your main code here, to run repeatedly:
  if(!done){
    jogSliderDownUntilTouch();
  }
  else{
    motion.jogArmBeltRight();
  }
  // if(Serial2.available()){
  //   Serial.println(Serial2.readString());
  // }

  // if(Serial.available()){
  //   Serial2.print(Serial.readString());
  // }
}
