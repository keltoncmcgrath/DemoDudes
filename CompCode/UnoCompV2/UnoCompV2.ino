// Include Libraries
#include <SoftwareSerial.h>


// Serial
SoftwareSerial mySerial(2, 3);  //TX & RX

// Data and Variables
int flag = 33;          //33 corresponds to an exclamation point serving as a good flag variable for block data
int num_blocks;



void setup() {
  // Begin serial comms
  Serial.begin(9600);    // USB Port
  mySerial.begin(9600);  // XBee
  Serial.println(F("Enter Block Info String:"));
}


void loop() {
  // Repeat each time info is sent to serial moniter
  while(Serial.available()){
    // Read num blocks on chassis
    while (true) {                          // Wait for user input of number of blocks (atoi if read as char)
<<<<<<< HEAD
      if (Serial.available() >= 4 && Serial.read() == flag) {   // The first bit corresponds to the flag
        //num_blocks = Serial.read() - '0';   // Serial.read() reads char into an ascii value, subtract ascii for '0' to convert to actual num
        num_blocks = Serial.readStringUntil(flag).toInt();
=======
      if (Serial.available() >= 2 && Serial.read() == flag) {   // The first bit corresponds to the flag
        num_blocks = Serial.readStringUntil(flag).toInt();   // Serial.read() reads char into an ascii value, subtract ascii for '0' to convert to actual num
>>>>>>> revamp
        break;
      } // end if
    } // end while
    
    // Read block data from serial monitor
    char block_data[4 * num_blocks + 4] = {};  // Create array to store block data
    block_data[0] = flag;
    block_data[1] = String(num_blocks)[0];
    block_data[2] = String(num_blocks)[1];
    block_data[3] = flag;
    for (int i = 0; i < num_blocks; i++) {    // Loop over number of blockes reported
      while (true) {                    // Wait for user input of block data
        if (Serial.available() >= 4) {  // Read data if available
          block_data[i * 4 + 4] = Serial.read();
          block_data[i * 4 + 5] = Serial.read();
          block_data[i * 4 + 6] = Serial.read();
          block_data[i * 4 + 7] = Serial.read();
          break;
        } // end if
      } // end while
      if (i == num_blocks - 1) {  // Add 'null character' at last index to signal end of array
        block_data[i * 4 + 8] = '\0';
      } // end if
    } // end for

    // Send block data to mega
<<<<<<< HEAD
    mySerial.write(flag);
    for (int i = 0; i < (sizeof(block_data) / sizeof(char)); i++) { //num_blocks * 4 + 2
=======
    for (int i = 0; i < num_blocks * 4 + 4; i++) {
>>>>>>> revamp
      mySerial.write(block_data[i]);
      Serial.write(block_data[i]);
    } // end for
    Serial.println();
    Serial.println(F("Enter Block Info String:"));
    break;
  } // end while
} // end loop
