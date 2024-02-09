// Include Libraries
#include <SoftwareSerial.h>

// Serial
SoftwareSerial mySerial(2, 3);  //TX & RX

// Data and Variables
int flag = 33;              //33 corresponds to an exclamation point serving as a good flag variable for block data
char demo_command;          //Initializing a variable for user input
char send_data[3] = {};     //Initializing vector to send command 


void setup() {
  // Begin serial comms
  Serial.begin(9600);    // USB Port
  mySerial.begin(9600);  // XBee

  // Ask which demo we are doing
  Serial.println();
  Serial.println(F("What action are we demo-ing:"));
  Serial.println(F("f - Drive Foward"));
  Serial.println(F("b - Drive Backwards"));
  Serial.println(F("r - Turn Right"));
  Serial.println(F("l - Turn Left"));
 
}


void loop() {
  // 
  if (Serial.available() == 2) {        //The first bit corresponds to the demo command and the second bit corresponds to the enter command
    demo_command = Serial.read();       //Serial.read() reads the character of demo_command
    Serial.read();                      //Reads the enter command again to remove it from the buffer
    // Write the demo command from the uno to the mega
    send_data[0] = flag;                //Setting the first bucket of the data array to the flag variable
    send_data[1] = demo_command;        //Setting the second bucket of the data array to the demo command
    send_data[2] = '\0';                //Setting the last bucket of the data array to a null character for concise printing 
    mySerial.write(send_data);          //Sending data
  }

}
