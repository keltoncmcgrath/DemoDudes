// Block Data Storage
char red1[] = {};
char red2[] = {};
char blue[] = {};
char yellow[] = {};

// Variables
int num_blocks;
int flag = 33;


void setup() {
  // Begin serial communication
  Serial.begin(9600);
  Serial3.begin(9600);
}


void loop() {
  // Read incoming block information
  if(Serial3.available()>=2){                         // Checks for start flag and number of blocks
    if(Serial3.read() == flag){                       // Checks if the first byte is the start flag
      num_blocks = Serial3.read() - '0';              // Store number of blocks
      while(true){                                    // While we know data is sending
        if(Serial3.available()>=num_blocks*4){        // If all the data is sent
          for(int i=0; i<num_blocks; i++){            // Read the data block by block
            Serial.write(Serial3.read());
            Serial.write(Serial3.read());
            Serial.write(Serial3.read());
            Serial.write(Serial3.read());
          }
          break;
        }
      }
    }
  }
//   if(Serial3.available()){
//     Serial.write(rc);
//   }
}
