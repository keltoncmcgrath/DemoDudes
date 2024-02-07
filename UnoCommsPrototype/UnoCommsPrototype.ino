// Include Libraries
#include <SoftwareSerial.h>

// Serial
SoftwareSerial mySerial(2, 3);  //TX & RX

// Data and Variables
int flag = 33;          //33 corresponds to an exclamation point serving as a good flag variable
int num_blocks; 


void setup() {
  // Begin serial comms
  Serial.begin(9600);    // USB Port
  mySerial.begin(9600);  // XBee

  // Read num blocks on chassis
  Serial.println(F("How Many Blocks on Chassis?"));
  while (true) {                          // Wait for user input of number of blocks (atoi if read as char)
    if (Serial.available() == 2) {        //The first bit corresponds to the number inputed for the number of block and the second bit corresponds to the enter command
      num_blocks = Serial.read() - '0';   // Serial.read() reads char into an ascii value, subtract ascii for '0' to convert to actual num
      Serial.read();                      //Reads the enter command again to remove it from the buffer
      break;
    }
  }

  // Read block data from serial monitor
  char block_data[4 * num_blocks + 2] = {};  // Create array to store block data
  block_data[0] = flag;
  for (int i = 0; i < num_blocks; i++) {    // Loop over number of blockes reported
    Serial.print(F("Info of Block "));
    Serial.print(i + 1);
    Serial.println(F(":"));
    while (true) {                    // Wait for user input of block data
      if (Serial.available() == 5) {  // Read data if available
        block_data[i * 4 + 1] = Serial.read();
        block_data[i * 4 + 2] = Serial.read();
        block_data[i * 4 + 3] = Serial.read();
        block_data[i * 4 + 4] = Serial.read();
        Serial.read();
        break;
      }
    }
    if (i == num_blocks - 1) {  // Add 'null character' at last index to signal end of array
      block_data[i * 4 + 5] = '\0';
    }
  }

  // Send block data to mega
  for (int i = 0; i < num_blocks * 4 + 2; i++) {
    mySerial.write(block_data[i]);
    Serial.write(block_data[i]);
    if (i == 0) {
      mySerial.write(num_blocks + '0');
      Serial.write(num_blocks + '0');
    }
  }
}


void loop() {
  // TODO
}
