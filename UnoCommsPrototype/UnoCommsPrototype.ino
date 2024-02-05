// Include Libraries
#include <SoftwareSerial.h>

// Serial
SoftwareSerial mySerial(2, 3);  //TX & RX

// Input Data
int flag = 255;
int num_blocks = 0;

// Read Conditionals
bool num_block_loop = true;
bool block_info_loop = true;


void setup() {
  // Begin serial comms
  Serial.begin(9600);     // USB Port
  mySerial.begin(9600);   // XBee

  // Read num blocks on chassis
  Serial.println(F("How Many Blocks on Chassis?"));
  while(num_block_loop){
    if(Serial.available() == 1) {
      num_blocks = Serial.read();
      num_block_loop = false;
    }
  }
  char block_info[4*num_blocks] = {};

  // Read block info from serial monitor
  for(int i=0; i<int(num_blocks); i++){
    Serial.print(F("Info of Block "));
    Serial.print(i+1);
    Serial.println(F(":"));
    block_info_loop = true;
    while(block_info_loop){
      if(Serial.available() == 5){
        block_info[i*4+0] = Serial.read();
        block_info[i*4+1] = Serial.read();
        block_info[i*4+2] = Serial.read();
        block_info[i*4+3] = Serial.read();
        block_info_loop = false;
      }
    }  
  }

  Serial.println(block_info);
  
  
  
}

void loop() {
  // Data to send
  // TODO

  // Sending Data
  // TODO
  
}
