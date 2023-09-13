/*

  Bypassing serial communication between 2 devices, (BB)BlackBox 4x v2.10 GRBL controller and (Int)Interfac from Openbuilds.
  Type: UART TTL Serial
  Baud Rate: 115200

  You can write in the serial monitor, It receives from the serial port 0 (PC), sends to the serial port 1 (BB) and print.
  Receives from the serial port 1 (BB), sends to the serial port 2 (Int).
  Receives from the serial port 2 (Int), sends to the serial port 1 (BB).
  

  This example works only with boards with more than one serial like Arduino Mega, Due, Zero etc.

  The circuit:

  - BB attached to Serial port 1 (BB TX-TX1, BB RX-RX1)
  - Int attached to Serial port 2 (Int TX-RX2, Int RX-TX2)
  - Serial Monitor open on Serial port 0

  created 20 Jan 2023
  By Arturo Castillo

  NOTE: This code is sensitive to delays due to the recurrent need of update msgs between the BB and Int during presice serial communication.
  Adding operations or delays may disable the use of the Interface, for this reason the msgs between BB and Int are not printed.
  So, simultaneous PC and Int controll is only available is the Arduino is only dedicated to bypass the serial communication.
  
  As an alternative, toggle between these two modes:
  1.- use Arduino conected to the Blacbox to read sensors and control motors in closed loop
  2.- replace the Arduino-blackbox serial connection by Interface-blackbox conection to Setup the machine initial properties and control motors easier
  meanwhile, sensor information is available in the Arduino-PC USB serial connection through the serial monitor.

*/

void setup() {

  // initialize both serial ports:

  Serial.begin(115200);

  Serial1.begin(115200);

  Serial2.begin(115200);
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
    //Serial2.write(grblMSG);
    Serial.println(grblMSG);
    //Printing on PC constantly the grbl update msg (reply to 63 , ?) to update HMI screen, disabilitates the function of the HMI while PC commmands keep working.
    //Operations or delay above 15ms inside the loop disable the use of the HMI

   


   
  }

  // read from port 2, send to port 1:

  if (Serial2.available()) {

    int hmiMSG = Serial2.read();
    //Serial1.write(hmiMSG);
    //Serial.println(hmiMSG);

    /*
    if (hmiMSG != 63){
      Serial1.write(63);
      Serial1.write(hmiMSG);
      Serial.println(hmiMSG);
    } 
    // blocking from the set-up the 63 (? update) msg of the HMI disable its functionality, since they requiere initial continuous communication.
    // alternatively one can disable the msg later after set-up, and send ? from PC to enable sending 1 msg from the HMI during near to 5sec, in that case grbl msg can be also printed.
    */
  }
}
