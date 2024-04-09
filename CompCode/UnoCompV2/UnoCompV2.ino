// Include Libraries
#include <SoftwareSerial.h>
// Serial
SoftwareSerial mySerial(2, 3);  //TX & RX

// Data and Variables
int flag = 33;  //33 corresponds to an exclamation point serving as a good flag variable for block data
int num_blocks;

void setup() {
  // Begin serial comms
  Serial.begin(9600);    // USB Port
  mySerial.begin(9600);  // XBee
}


void loop() {
  // TODO
if (Serial.available()){
        mySerial.write(Serial.read());
    if (Serial.read() == flag) {
      num_blocks = Serial.read();
      num_blocks = num_blocks - 1;
      char block_data[4 * num_blocks + 2] = {};  // Create array to store block data
      block_data[0] = flag;
      for (int i = 0; i < num_blocks; i++) {  // Loop over number of blockes reported
        block_data[i * 4 + 1] = Serial.read();
        block_data[i * 4 + 2] = Serial.read();
        block_data[i * 4 + 3] = Serial.read();
        block_data[i * 4 + 4] = Serial.read();
        if (i == num_blocks - 1) {  // Add 'null character' at last index to signal end of array
          block_data[i * 4 + 5] = '\0';
        }
      }
      // Send block data to mega
      //mySerial.write(flag);
      //for (int i = 0; i < num_blocks * 4 + 2; i++) {
        //mySerial.write(block_data[i]);
       // if (i == 0) {
          //mySerial.write(num_blocks);
        //}
      //}
    }
}
}
