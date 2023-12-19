#include "fsm.h"

fsm fsmFlow;


/************************************************
 * This is the starting set up of the program.
 ***********************************************/
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(2000);
  fsmFlow.setUp();
  while (Serial1.available()) {
    String msg = Serial1.readString();
    Serial.println(msg);
  }
}


/************************************************
 * This loop will keep running during the program execution.
 ***********************************************/
void loop() {
  fsmFlow.fsmMain();
}
