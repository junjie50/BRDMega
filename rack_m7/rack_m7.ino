#include "fsm.h"

fsm fsmFlow;

/************************************************
 *This is the starting set up of the program.
 ***********************************************/
void setup() {
  Serial.begin(115200);
  //Serial1.begin(115200);
  
  fsmFlow.setUp();
  delay(1000);
}

/************************************************
 * This loop will keep running during the program execution.
 ***********************************************/
void loop() {
  fsmFlow.fsmMain();
}