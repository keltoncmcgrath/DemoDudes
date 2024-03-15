// Include Libraries
#include <SoftwareSerial.h>

// Serial
SoftwareSerial mySerial(2, 3);  //TX & RX

// Data and Variables
int flag = 33;      //33 corresponds to an exclamation point serving as a good flag variable for block data
char demo_command;  //Initializing a variable for user input


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
  Serial.println(F("o - Travel a Certain Arc Length"));
  Serial.println(F("a - Move Arm Servo Up and Down"));
  Serial.println(F("s - Move Shovel Servo Up and Down"));
  Serial.println(F("i - Follow a Line"));
  Serial.println(F("d - Drive and Stop a Distance Away from an Object"));
  Serial.println(F("m - Detect Magnetic Field"));
  Serial.println(F("c - Detect Color of Block"));
  Serial.println(F("g - Perform Beginning Sequence"));
}


void loop() {
  //
  if (Serial.available() == 2) {   //The first bit corresponds to the demo command and the second bit corresponds to the enter command
    demo_command = Serial.read();  //Serial.read() reads the character of demo_command
    Serial.read();                 //Reads the enter command again to remove it from the buffer
    // Write the demo command from the uno to the mega
    mySerial.write(flag);
    mySerial.write(demo_command);
  }
}
