#include "DualTB9051FTGMotorShield.h"
#include <Encoder.h> 
// Creates an object that is easier to call than Dual....MotorShield
DualTB9051FTGMotorShield md;

//Initializing Variables
float duration = 1500;        // Setting time to run                       
float speed = 400;            // Setting Motor Speed

int flag = 33;                // Setting flag variables
char command;                 // Setting the command variable

int encoder1_pinA = 20;       // Declaring econder1 pins
int encoder1_pinB = 21;       // Declaring econder1 pins
int encoder2_pinA = 18;       // Declaring econder2 pins
int encoder2_pinB = 19;       // Declaring econder2 pins
int encoder1_count;           // Initializing encoder1 count values
int encoder2_count;           // Initializing encoder2 count values
 

// Motor on function
void MotorOn( float speed, float duration){
  // Sets Speed
  md.setM2Speed(speed); //Setting motor 2's speed
  md.setM1Speed(speed); //Setting motor 1/s speed
  delay(duration);      //Keeping the motors on for a set duration
  md.setM2Speed(0);     //Turning motors2 off
  md.setM1Speed(0);     //Turning motor1 off
}


void setup() {
  //Serial Communication
  Serial.begin(9600);
  Serial3.begin(9600);

  //Initializing and Enabling Driver Shield
  md.init();
  md.enableDrivers();

  //Initialzing encoder objects
  Encoder encoder1(encoder1_pinA,encoder1_pinB);
  Encoder encoder2(encoder2_pinA,encoder2_pinB);

  MotorOn(speed,duration);
  encoder1_count = encoder1.read();
  encoder2_count = encoder2.read();
  Serial.print(encoder1_count);
  Serial.print(encoder2_count);
}

void loop() {
  if(Serial3.available()>=2){
    if(Serial3.read() == flag){
      command = Serial3.read();
      Serial.write(command);
    }
  }
  switch(command) {
    case 'f': 
      MotorOn(speed,duration);
      break;
    case 'b':
      MotorOn(-speed,duration);
      break;
    case 'r':
      //TO DO 
      break;
    case 'l':
      //TO DO
      break;
  }
  command = 0;
}
