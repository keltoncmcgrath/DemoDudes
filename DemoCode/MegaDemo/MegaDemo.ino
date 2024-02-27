// Include Libraries
#include "DualTB9051FTGMotorShieldUnoMega.h"
#include <Encoder.h>
#include <Servo.h>
#include <PWMServo.h>
#include <QTRSensors.h>

// Creates an object that is easier to call than Dual....MotorShield
DualTB9051FTGMotorShieldUnoMega md;

// Create Servo Objects
// Servo arm_servo;
// Servo shovel_servo;
PWMServo arm_servo;
PWMServo shovel_servo;

// Create IR Sensor Object
QTRSensors qtr;

// Action Vars
long num_stripes = 131 * 64;    // Total encoder stripes
int duration = 1500;            // Setting time to run
float speed = 300;              // Setting Motor Speed
float turn_speed;
float dist_desired = 20;        // cm
float dist_val;
float a = exp(7.502420862);     //distance sensor lin fit variable
float b = -0.9238074889;        //distance sensor lin fit variable
float dist_actual; 

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
int arm_servo_pin = 23; 
int shovel_servo_pin = 22;
// int arm_servo_pin = 13;
//int shovel_servo_pin = 12;
int dist_pin = A7;
const uint8_t ir_pins[] = {24, 25, 26, 28, 27, 29, 30, 31};

// IR Array Variables
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count];
int ir_bias[] = {166, 58, 58, 106, 58, 106, 106};
int ir_unbiased[ir_sensor_count];
float ir_sensor_spacing[] = {0, 0.8, 1.6, 2.4, 3.2, 4.0, 4.8, 5.6};
float ir_dist_desired = 2.8;
float ir_dist_actual;
float ir_error;
int m1s;
int m2s;
int kp = 150;
float num = 0;
float den = 0;

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

void DistSense(float dist_desired, float speed){
  md.setSpeeds(speed, speed+10);
  dist_val = analogRead(dist_pin);
  dist_actual = pow((dist_val/a),(1/b));
  if (dist_actual <= dist_desired){
    md.setSpeeds(0, 0);
  }
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
  delay(1500);
  arm_servo.write(90);
}

// Shovel Servo Function
void ShovelServo(){
  shovel_servo.write(0);
  delay(2000);
  shovel_servo.write(180);
  delay(1500);
  shovel_servo.write(90); 
}

// Line Following Demo Function
void LineFollow(){
  // Read sensor values and take bias off
  qtr.read(ir_values);
  for(int i=0; i<ir_sensor_count; i++){
    ir_unbiased[i] = ir_values[i] - ir_bias[i];
    Serial.print(ir_values[i]);
    Serial.print("\t");
  }
  Serial.println();

  // Calculate distance of line from deisred distance
  for(int i=0; i<ir_sensor_count; i++){
    num = num + (ir_unbiased[i] * ir_sensor_spacing[i]);
    den = den + ir_unbiased[i];
  }
  ir_dist_actual = num/den; 
  num = 0;
  den = 0;

  // Calculate error and control speed
  ir_error = ir_dist_desired - ir_dist_actual;
  m1s = speed - kp*ir_error;
  m2s = speed + kp*ir_error;
  // Serial.println(ir_error);
  md.setSpeeds(m1s, m2s);
}


void setup() {
  //Serial Communication
  Serial.begin(9600);
  Serial2.begin(9600);

  //Initializing and Enabling Driver Shield
  md.init();
  md.enableDrivers();

  // Communicate
  Serial.println("Ready for signal...");

  // Attatch Pins to Servo Objects
  arm_servo.attach(arm_servo_pin);
  shovel_servo.attach(shovel_servo_pin);

  // Setup IR Array
  qtr.setTypeRC();
  qtr.setSensorPins(ir_pins, ir_sensor_count);
}

void loop() {
  if(Serial2.available()>=2){
    if(Serial2.read() == flag){
      command = Serial2.read();
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
    case 'd':
      DistSense(dist_desired, speed);
      break;
    case 'i':
      while(true){
        LineFollow();
      }
      break;
      
  }
  command = 0;
}
