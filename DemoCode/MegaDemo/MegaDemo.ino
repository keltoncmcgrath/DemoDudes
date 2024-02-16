// Include Libraries
#include "DualTB9051FTGMotorShield.h"
#include <Encoder.h>
#include <Servo.h>

// Creates an object that is easier to call than Dual....MotorShield
DualTB9051FTGMotorShield md;

// Create Servo Objects
Servo arm_servo;
Servo shovel_servo;

// Action Vars
long num_stripes = 131 * 64;    // Total encoder stripes
int duration = 1500;            // Setting time to run
float speed = 400;              // Setting Motor Speed
float turn_speed;

// Comms Vars
int flag = 33;                // Setting flag variables
char command;                 // Setting the command variable

// Pin Vars
int encoder1_pinA = 20;       // Declaring econder1 pins
int encoder1_pinB = 21;       // Declaring econder1 pins
int encoder2_pinA = 18;       // Declaring econder2 pins
int encoder2_pinB = 19;       // Declaring econder2 pins
int encoder1_count;           // Initializing encoder1 count values
int encoder2_count;           // Initializing encoder2 count values
int arm_servo_pin = 22; 
int shovel_servo_pin = 23;

//Initialzing encoder objects
Encoder encoder1(encoder1_pinA,encoder1_pinB);
Encoder encoder2(encoder2_pinA,encoder2_pinB);


// Motor on function
void MotorOn(float speed, float duration){
  // Sets Speed
  if(speed<0){
    md.setM2Speed(speed+10); //Setting motor 2's speed
    md.setM1Speed(speed); //Setting motor 1's speed
  }
  else{
    md.setM2Speed(speed); //Setting motor 2's speed
    md.setM1Speed(speed-10); //Setting motor 1's speed
  }
  delay(duration);      //Keeping the motors on for a set duration
  md.setM2Speed(0);     //Turning motors2 off
  md.setM1Speed(0);     //Turning motor1 off
}

// Turming function
void Turn(char command){
  double stripe_turning = num_stripes * 0.6786;    // Initializing the number of stripes we want to turn for turning
  if(command == 'r'){
    turn_speed = 200;
  }
  else if(command == 'l'){
    turn_speed = -200;
  }
  encoder1.write(0);
  encoder1_count = encoder1.read();
  encoder2.write(0);
  encoder2_count = encoder2.read();
  md.setM1Speed(turn_speed);
  md.setM2Speed(-turn_speed);
  while((abs(encoder1_count) <= stripe_turning) || (abs(encoder2_count) <= stripe_turning)){
  encoder1_count = encoder1.read();
  encoder2_count = encoder2.read();
  }
  md.setM1Speed(0);
  md.setM2Speed(0);
}

// Arms Servo Function
void ArmServo() {
  arm_servo.write(0);
  delay(2000);
  arm_servo.write(180); 
}

// Shovel Servo Function
void ShovelServo(){
  shovel_servo.write(0);
  delay(2000);
  shovel_servo.write(180); 
}


void setup() {
  //Serial Communication
  Serial.begin(9600);
  Serial3.begin(9600);

  //Initializing and Enabling Driver Shield
  md.init();
  md.enableDrivers();

  Serial.println("Ready for signal...");

  arm_servo.attach(arm_servo_pin);
  shovel_servo.attach(shovel_servo_pin); 
}

void loop() {
  if(Serial3.available()>=2){
    if(Serial3.read() == flag){
      command = Serial3.read();
      Serial.write(command);
      Serial.println();
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
      Turn(command);
      break;
    case 'l':
      Turn(command);
      break;
    case 'a':
      ArmServo();
      break;
    case 's':
      ShovelServo();
      break; 
  }
  command = 0;
}
