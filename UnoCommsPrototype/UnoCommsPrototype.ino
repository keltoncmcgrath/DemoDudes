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
    if(Serial.available() == 1) {
      num_blocks = Serial.read() - '0';   // Serial.read() reads char into an ascii value, subtract ascii for '0' to convert to actual num
      break;
    }
  }
  char block_info[4*num_blocks] = {};
  Serial.println(int(num_blocks));

  // Read block info from serial monitor
  for(int i=0; i<int(num_blocks); i++){
    Serial.print(F("Info of Block "));
    Serial.print(i+1);
    Serial.println(F(":"));
    while(true){
      if(Serial.available() == 5){
        Serial.read();
        block_info[i*4+0] = Serial.read();
        block_info[i*4+1] = Serial.read();
        block_info[i*4+2] = Serial.read();
        block_info[i*4+3] = Serial.read();
        break;
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
