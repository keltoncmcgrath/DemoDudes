// Include Libraries
#include <SoftwareSerial.h>

// Serial
SoftwareSerial mySerial(2, 3);  //TX & RX

// Input Data
int flag = 255;
int num_blocks = 0;


void setup() {
  // Begin serial comms
  Serial.begin(9600);     // USB Port
  mySerial.begin(9600);   // XBee

  // Read num blocks on chassis
  Serial.println(F("How Many Blocks on Chassis?"));
  while(true){
    if(Serial.available() == 2) {
      num_blocks = Serial.read() - '0';   // Serial.read() reads char into an ascii value, subtract ascii for '0' to convert to actual num
      Serial.read();
      break;
    }
  }

  // Read block data from serial monitor
  char block_data[4*num_blocks] = {};
  for(int i=0; i<int(num_blocks); i++){
    Serial.print(F("Info of Block "));
    Serial.print(i+1);
    Serial.println(F(":"));
    while(true){
      if(Serial.available() == 5){
        block_data[i*4+0] = Serial.read();
        block_data[i*4+1] = Serial.read();
        block_data[i*4+2] = Serial.read();
        block_data[i*4+3] = Serial.read();
        Serial.read();
        break;
      }
    }
  }

  // Send block data to mega
  mySerial.write(block_data);
}


void loop() {
  // TODO
  
}
