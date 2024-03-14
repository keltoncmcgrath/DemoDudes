// Include Libraries
#include "DualTB9051FTGMotorShieldUnoMega.h"
#include <Encoder.h>
#include <Servo.h>
#include <PWMServo.h>
#include <QTRSensors.h>

// Creates an object that is easier to call than Dual....MotorShield
DualTB9051FTGMotorShieldUnoMega md;

// Create Servo Objects
Servo arm_servo;
Servo shovel_servo;
// PWMServo arm_servo;
// PWMServo shovel_servo;

// Create IR Sensor Object
QTRSensors qtr;

// Moving Variables
long num_stripes = 131 * 64;    // Total encoder stripes
int duration = 1500;            // Setting time to run
float speed = 200;                    // Setting Motor Speed
float turn_speed;

// Comms Vars
int flag = 33;                // Setting flag variables
char command;                 // Setting the command variable

// Pin Vars
int encoder2_pinA = 20;       // Declaring econder1 pins
int encoder2_pinB = 21;       // Declaring econder1 pins
int encoder1_pinA = 19;       // Declaring econder2 pins
int encoder1_pinB = 18;       // Declaring econder2 pins
int encoder1_count;           // Initializing encoder1 count values
int encoder2_count;           // Initializing encoder2 count values
int arm_servo_pin = 22; 
int shovel_servo_pin = 23;
int dist_pin = A8;
const uint8_t ir_pins[] = {24, 25, 26, 28, 27, 29, 30, 31};
int mag_pin = A9;
int blue_pin = 33;
int red_pin = 35;
int green_pin = 34;
int photo_trans_pin = A10;

// General Vars
float t;
float t_start;

// Arm Servo Variables
int arm_home = 93;
int arm_angle_start;
int arm_angle_final;
float arm_angle_des;
float arm_t_final = 2;
float arm_tol = 0.1;

// Shovel Servo Variables
int shov_angle_start = 90;
int shov_angle_final = 90;
float shov_angle_des;
float shov_t_final = 10;

// IR Array Vars
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
int kp = 75;
float num = 0;
float den = 0;

// Rane Finder Vars
float dist_desired = 20;      // cm
float dist_val;
float a = exp(7.453976699);   //distance sensor lin fit variable
float b = -0.907499336;       //distance sensor lin fit variable
float dist_actual;
bool stop = false;

//Initializing Hall Effect Sensor
float mag_val;

// Color Sensor Variables
bool is_color = false;
int red_sum = 0; 
int green_sum = 0;
int blue_sum = 0;
int red_val;
int green_val;
int blue_val;
float red_avg[3];
float green_avg[3];
float blue_avg[3];
float red_std[3] = {0, 0, 0};
float green_std[3] = {0, 0, 0};
float blue_std[3] = {0, 0, 0};
int color_delay_time = 10;
const int color_samples = 100;
int red_calibration_vals[color_samples];
int green_calibration_vals[color_samples];
int blue_calibration_vals[color_samples];
int color_ranges[3][3][2];
int color_vals[color_samples][3];

// Mag Sensor Variables
int mag_flag = 600;

//Initialzing encoder objects
Encoder encoder1(encoder1_pinA,encoder1_pinB);
Encoder encoder2(encoder2_pinA,encoder2_pinB);


// Motor on function
void MotorOn(){
  // Sets Speed
  md.setSpeeds(speed, speed);
  delay(duration);      //Keeping the motors on for a set duration
  md.setSpeeds(0, 0);
}

void DistSense(){
  md.setSpeeds(speed, speed+10);
  dist_val = analogRead(dist_pin);
  dist_actual = pow(dist_val/a, 1/b);
  if(dist_actual <= dist_desired){
    md.setSpeeds(0, 0);
    stop = true;
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
  t_start = millis();
  arm_angle_start = arm_servo.read();
  while(arm_servo.read() != arm_angle_final){
    t = (millis()-t_start) / 1000;
    if(arm_angle_start > arm_angle_final){
      arm_angle_des = ((arm_angle_start-arm_angle_final) / (1+exp((10/arm_t_final)*t-5))) + arm_angle_final;
    }
    if(arm_angle_start < arm_angle_final){
      arm_angle_des = (arm_angle_final-arm_angle_start) / (1+exp(-(10/arm_t_final*t)+5)) + arm_angle_start + 1;
    }
    arm_servo.write(arm_angle_des);
    Serial.print(arm_servo.read());
    Serial.print('\t');
    Serial.print(arm_angle_des);
    Serial.print('\t');
    Serial.println(t);
  }
  delay(1000);
}

// Shovel Servo Function
void ShovelServo(){
  t = (millis()-t_start) / 1000;
  shov_angle_start = shovel_servo.read();
  if(shov_angle_start > shov_angle_final){
    shov_angle_des = ((shov_angle_start-shov_angle_final) / (1+exp(t-shov_t_final/2))) + shov_angle_final;
  }
  if(shov_angle_start < shov_angle_final){
    shov_angle_des = (shov_angle_final-shov_angle_start) / (1+exp(-t+shov_t_final/2)) + shov_angle_start;
  }
  shovel_servo.write(shov_angle_des);
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

void MagSense(){
  mag_val = analogRead(mag_pin);
  Serial.println(mag_val);
  if(mag_val > mag_flag){
    Serial.println("Ramp down!")
  }
}

void ColorSense(){
  Serial.println("Fart");
  // Reading Red Light 
  digitalWrite(red_pin, LOW);
  delay(color_delay_time);
  red_val = analogRead(photo_trans_pin);
  digitalWrite(red_pin, HIGH);

  // Reading Green Light
  digitalWrite(green_pin, LOW);
  delay(color_delay_time);
  green_val = analogRead(photo_trans_pin);
  digitalWrite(green_pin, HIGH);

  // Reading Blue Light
  digitalWrite(blue_pin, LOW);
  delay(color_delay_time);
  blue_val = analogRead(photo_trans_pin);
  digitalWrite(blue_pin,HIGH);

  for(int i=0; i<3; i++){
    if(red_val > color_ranges[i][0][0] & red_val < color_ranges[i][0][1]){
      if(green_val > color_ranges[i][1][0] & green_val < color_ranges[i][1][1]){
        if(blue_val > color_ranges[i][2][0] & blue_val < color_ranges[i][2][1]){
          is_color = true;
          if(i==0){
            Serial.println("Block is RED");
          }
          else if(i==1){
            Serial.println("Block is YELLOW");
          }
          else{
            Serial.println("Block is BLUE");
          }
        }
      }    
    }
  }
  if(!is_color){
    Serial.println("Block Color Not Found");
    Serial.print(red_val);
    Serial.print('\t');
    Serial.print(green_val);
    Serial.print('\t');
    Serial.println(blue_val);
  }
  is_color = false;
}

void ColorCalibration(){
  for(int j=0; j<3; j++){
    if(j == 0){
      Serial.println("Place Red Block on Sensor");
    }
    else if(j == 1){
      Serial.println("Place Yellow Block on Sensor");
    }
    else{
      Serial.println("Place Blue Block on Sensor");
    }
    delay(5000);
    Serial.println("Reading Color Values...");
    for(int i=0; i<color_samples; i++){
      digitalWrite(red_pin, LOW);
      delay(color_delay_time);
      red_val = analogRead(photo_trans_pin);
      red_calibration_vals[i] = red_val;
      red_sum += red_val;
      digitalWrite(red_pin, HIGH);

      digitalWrite(green_pin, LOW);
      delay(color_delay_time);
      green_val = analogRead(photo_trans_pin);
      green_calibration_vals[i] = green_val;
      green_sum += green_val;
      digitalWrite(green_pin, HIGH);

      digitalWrite(blue_pin, LOW);
      delay(color_delay_time);
      blue_val = analogRead(photo_trans_pin);
      blue_calibration_vals[i] = blue_val;
      blue_sum += blue_val;
      digitalWrite(blue_pin, HIGH);    
    }

    // Calculate Avgerages
    red_avg[j] = red_sum/color_samples;
    green_avg[j] = green_sum/color_samples;
    blue_avg[j] = blue_sum/color_samples;

    // Calculate Standard Deviations
    for(int i=0; i<color_samples; i++){
      red_std[j] += pow((red_calibration_vals[i] - red_avg[j]), 2);
      green_std[j] += pow((green_calibration_vals[i] - green_avg[j]), 2);
      blue_std[j] += pow((blue_calibration_vals[i] - blue_avg[j]), 2);
    }
    red_std[j] = sqrt(red_std[j]/(color_samples-1));
    green_std[j] = sqrt(green_std[j]/(color_samples-1));
    blue_std[j] = sqrt(blue_std[j]/(color_samples-1));

    // Set Sums to Zero
    red_sum = 0;
    green_sum = 0;
    blue_sum = 0;

    // Find Ranges of Values for Each Block
    for(int i=0; i<3; i++){
      // Red
      color_ranges[i][0][0] = red_avg[i]-4*red_std[i];
      color_ranges[i][0][1] = red_avg[i]+4*red_std[i];
      // Green
      color_ranges[i][1][0] = green_avg[i]-4*green_std[i];
      color_ranges[i][1][1] = green_avg[i]+4*green_std[i];
      // Blue
      color_ranges[i][2][0] = blue_avg[i]-4*blue_std[i];
      color_ranges[i][2][1] = blue_avg[i]+4*blue_std[i];
    }
  }
  for(int i=0; i<3; i++){
    Serial.print(color_ranges[i][0][0]);
    Serial.print('\t');
    Serial.println(color_ranges[i][0][1]);

    Serial.print(color_ranges[i][1][0]);
    Serial.print('\t');
    Serial.println(color_ranges[i][1][1]);

    Serial.print(color_ranges[i][2][0]);
    Serial.print('\t');
    Serial.println(color_ranges[i][2][1]);
    Serial.println();
  }
}

void setup() {
  //Serial Communication
  Serial.begin(9600);
  Serial2.begin(9600);
  while(!Serial){
    ;
  }

  // Initilaize Start Time
  t_start = millis();

  //Initializing and Enabling Driver Shield
  md.init();
  md.enableDrivers();

  // Attatch Pins to Servo Objects
  arm_servo.attach(arm_servo_pin);
  shovel_servo.attach(shovel_servo_pin);

  // Setup IR Array
  qtr.setTypeRC();
  qtr.setSensorPins(ir_pins, ir_sensor_count);

  // Set Color Sensor Pins
  pinMode(blue_pin, OUTPUT);  digitalWrite(blue_pin, HIGH);
  pinMode(red_pin, OUTPUT);   digitalWrite(red_pin, HIGH);
  pinMode(green_pin,OUTPUT);  digitalWrite(green_pin, HIGH);

  // Automated Color Calibration
  Serial.println("Calibrate Color Sensor? (y/n)");
  while(true){
    if(Serial.available()){
      if(Serial.read() == 'y'){
        Serial.println("Calibrating...");
        ColorCalibration();
        Serial.println("Done!");
        break;
      }
      else{
        break;
      }
    }
  }

  // Communicate Ready for Signal
  Serial.println("Ready for Signal...");
  
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
      speed = 400;
      MotorOn();
      break;
    case 'b':
      speed = -400;
      MotorOn();
      break;
    case 'r':
      Turn(command);
      break;
    case 'l':
      Turn(command);
      break;
    case 'a':
      arm_angle_final = 15;
      ArmServo();
      arm_angle_final = 93;
      ArmServo();
      break;
    case 's':
      ShovelServo();
      break; 
    case 'd':
      while(!stop){
        DistSense();
      }
      stop = false;
      break;
    case 'i':
      while(true){
        LineFollow();
      }
      break;
    case 'm':
      while(true){
        MagSense();
      }
      break;
    case 'c':
      ColorSense();
      break; 
  }
  command = 0;
}
