// Include Libraries
#include "DualTB9051FTGMotorShieldUnoMega.h"
#include <Encoder.h>
#include <Servo.h>
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
const uint8_t ir_pins[] = {24, 25, 26, 27, 28, 29, 30, 31};
int mag_pin = A9;
int blue_pin = 33;
int green_pin = 34;
int red_pin = 35;
int photo_trans_pin = A10;

// General Vars
float t;
float t_old;
float t_start;
float delta_T;
float pi = 3.1415927;

// Arm Servo Variables
int arm_home = 93;
int arm_angle_start;
int arm_angle_final;
float arm_angle_des;
float arm_t_final = 1;
float arm_tol = 0.1;

// Shovel Servo Variables
int shov_angle_start;
int shov_angle_final;
float shov_angle_des;
float shov_t_final = 2;

// IR Array Vars
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count];
int ir_bias[] = {144, 93, 93, 93, 136, 93, 98, 119};
int ir_unbiased[ir_sensor_count];
float ir_sensor_spacing[] = {0, 0.8, 1.6, 2.4, 3.2, 4.0, 4.8, 5.6};
float ir_dist_desired = 2.8;
float ir_dist_actual;
float ir_error;
int m1s;
int m2s;
int kp = 100;
float num = 0;
float den = 0;
int speed = 200;

// Rane Finder Vars
float dist_desired = 20;      // cm
float dist_val;
float dist_actual;
float a = exp(7.453976699);   //distance sensor lin fit variable
float b = -0.907499336;       //distance sensor lin fit variable
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

// Travel Variables
float theta1;
float theta2;
float theta1_final;
float theta2_final;
float theta1_des = 0;
float theta2_des = 0;
float omega1_des;
float omega2_des;
long counts1;
long counts2;
float speed1;
float speed2;
float KP = 300;
int counts_per_rev = 64;
int gear_ratio = 131;
float dist_final = 40;         // cm
float turn_angle_final = PI/2;   // rad
float arc_radius = 0.8;        // cm
float arc_angle_final = pi/2;  // rad
float time_final = 2;          // s
float wheel_radius = 3.5;      // cm 
float wheel_dist = 20.8;       // cm

//Initialzing encoder objects
Encoder encoder1(encoder1_pinA,encoder1_pinB);
Encoder encoder2(encoder2_pinA,encoder2_pinB);


// Motor on function
void MotorOn(){
  // Reset Variables
  t_start = millis();
  t_old = 0;
  counts1 = 0;
  counts2 = 0;
  theta1_des = 0;
  theta2_des = 0;
  encoder1.write(0);
  encoder2.write(0);

  // Set theta final and omega besed on direction of travel
  if(command == 'f'){
    theta1_final = dist_final/wheel_radius;
    theta2_final = dist_final/wheel_radius;
  }
  else{
    theta1_final = -dist_final/wheel_radius;
    theta2_final = -dist_final/wheel_radius;
  }
  omega1_des = theta1_final/time_final;
  omega2_des = theta2_final/time_final;

  // Trajectory and control loop
  while(abs(theta1_des) < abs(theta1_final) && abs(theta2_des) < abs(theta2_final)){
    // Update varaibles
    t = (millis()-t_start)/1000;
    delta_T = t - t_old;
    counts1 = encoder1.read();
    counts2 = encoder2.read();
    theta1 = float(counts1*2*pi) / (gear_ratio * counts_per_rev);
    theta2 = float(counts2*2*pi) / (gear_ratio * counts_per_rev);

    // Calculate new desired thetas
    theta1_des += omega1_des*delta_T;
    theta2_des += omega2_des*delta_T; 
    if(abs(theta1_des)>=abs(theta1_final)){
      theta1_des = theta1_final;
    }
    if(abs(theta2_des)>=abs(theta2_final)){
      theta2_des = theta2_final;
    }

    // Control and constrain speed
    speed1 = KP * (theta1_des-theta1);
    speed2 = KP * (theta2_des-theta2);
    speed1 = constrain(speed1, -400, 400);
    speed2 = constrain(speed2, -400, 400);

    // Set motor speeds and record old time
    md.setSpeeds(speed1,speed2); 
    t_old = t;
  }
  md.setSpeeds(0, 0);
}

// Turming function
void Turn(){
  // Reset Variables
  t_start = millis();
  encoder1.write(0);
  encoder2.write(0);
  counts1 = 0;
  counts2 = 0;
  theta1_des = 0;
  theta2_des = 0;
  t_old = 0;

  // Set theta final and omega besed on direction of travel
  if(command == 'r'){
    theta1_final = turn_angle_final * (wheel_dist/2) / wheel_radius;
    theta2_final = -turn_angle_final * (wheel_dist/2) / wheel_radius;
  }
  else{
    theta1_final = -turn_angle_final * (wheel_dist/2) / wheel_radius;
    theta2_final = turn_angle_final * (wheel_dist/2) / wheel_radius;
  }
  omega1_des = theta1_final/time_final;
  omega2_des = theta2_final/time_final;

  // Trajectory and control loop
  while(abs(theta1_des) < abs(theta1_final) && abs(theta2_des) < abs(theta2_final)){
    // Update varaibles
    t = (millis()-t_start)/1000;
    delta_T = t - t_old;
    counts1 = encoder1.read();
    counts2 = encoder2.read();
    theta1 = float(counts1*2*pi) / (gear_ratio * counts_per_rev);
    theta2 = float(counts2*2*pi) / (gear_ratio * counts_per_rev);

    // Calculate new desired thetas
    theta1_des += omega1_des*delta_T;
    theta2_des += omega2_des*delta_T;

    if(abs(theta1_des)>=abs(theta1_final)){
      theta1_des = theta1_final;
    }
    if(abs(theta2_des)>=abs(theta2_final)){
      theta2_des = theta2_final;
    }

    // Control and constrain speed
    speed1 = KP * (theta1_des-theta1);
    speed2 = KP * (theta2_des-theta2); 
    speed1 = constrain(speed1, -400, 400);
    speed2 = constrain(speed2, -400, 400);

    Serial.print(theta1);
    Serial.print('\t');
    Serial.println(theta2);

    // Set motor speeds and record old time
    md.setSpeeds(speed1,speed2); 
    t_old = t;
  }
  md.setSpeeds(0, 0);
}

void Arc(){
  // Reset variables
  t_start = millis();
  encoder1.write(0);
  encoder2.write(0);
  counts1 = 0;
  counts2 = 0;
  theta1_des = 0;
  theta2_des = 0;
  t_old = 0;

  // Set theta final and omega besed on direction of travel
  theta1_final = arc_angle_final * arc_radius / wheel_radius;
  theta2_final = arc_angle_final * (arc_radius+wheel_dist) / wheel_radius;

  omega1_des = theta1_final/time_final;
  omega2_des = theta2_final/time_final;

  // Trajectory and control loop
  while(abs(theta1_des) < abs(theta1_final) && abs(theta2_des) < abs(theta2_final)){
    // Update varaibles
    t = (millis()-t_start)/1000;
    delta_T = t - t_old;
    counts1 = encoder1.read();
    counts2 = encoder2.read();
    theta1 = float(counts1*2*pi) / (gear_ratio * counts_per_rev);
    theta2 = float(counts2*2*pi) / (gear_ratio * counts_per_rev);


    // Calculate new desired thetas
    theta1_des += omega1_des*delta_T;
    theta2_des += omega2_des*delta_T; 

    if(abs(theta1_des)>=abs(theta1_final)){
      theta1_des = theta1_final;
    }
    if(abs(theta2_des)>=abs(theta2_final)){
      theta2_des = theta2_final;
    }

    // Control and constrain speed
    speed1 = KP * (theta1_des-theta1);
    speed2 = KP * (theta2_des-theta2);
    speed1 = constrain(speed1, -400, 400);
    speed2 = constrain(speed2, -400, 400);
    Serial.print(speed1);
    Serial.print("\t");
    Serial.println(speed2);


    // Set motor speeds and record old time
    md.setSpeeds(speed1,speed2); 
    t_old = t;
  }
  md.setSpeeds(0, 0);
}

// Arms Servo Function
void ArmServo(){
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
  t_start = millis();
  shov_angle_start = shovel_servo.read();
  while(shovel_servo.read() != shov_angle_final){
    t = (millis()-t_start) / 1000;
    if(shov_angle_start > shov_angle_final){
      shov_angle_des = ((shov_angle_start-shov_angle_final) / (1+exp((10/shov_t_final)*t-5))) + shov_angle_final;
    }
    if(shov_angle_start < shov_angle_final){
      shov_angle_des = (shov_angle_final-shov_angle_start) / (1+exp(-(10/shov_t_final*t)+5)) + shov_angle_start + 1;
    }
    shovel_servo.write(shov_angle_des);
    Serial.print(shovel_servo.read());
    Serial.print('\t');
    Serial.print(shov_angle_des);
    Serial.print('\t');
    Serial.println(t);
  }
  delay(1000);
}

// Line Following Demo Function
void LineFollow(){
  // Read sensor values and take bias off
  qtr.read(ir_values);
  for(int i=0; i<ir_sensor_count; i++){
    ir_unbiased[i] = ir_values[i] - ir_bias[i];
    // Serial.print(ir_unbiased[i]);
    // Serial.print('\t');
  }
  // Serial.println();

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
  Serial.print(m1s);
  Serial.print('\t');
  Serial.println(m2s);

  // Serial.println(ir_error);
  md.setSpeeds(m1s, m2s);
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

void MagSense(){
  mag_val = analogRead(mag_pin);
  Serial.println(mag_val);
  if(mag_val > mag_flag){
    Serial.println("Ramp down!");
  }
}

void ColorSense(){
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
  Serial.print(red_val);
  Serial.print('\t');
  Serial.print(green_val);
  Serial.print('\t');
  Serial.println(blue_val);

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

void Begin(){
  dist_val = 0;
  dist_desired = 10;

  for(int i=0; i<10; i++){
    dist_val += analogRead(dist_pin);
  }

  dist_val /= 10;
  dist_actual = pow(dist_val/a, 1/b);
  // Serial.println(dist_actual);

  while(dist_actual >= dist_desired){
    LineFollow();
    dist_val = analogRead(dist_pin);
    dist_actual = pow(dist_val/a, 1/b);
    // Serial.println(dist_actual);
  }
  md.setSpeeds(0,0);
  arm_t_final = 1;
  arm_angle_final = 73;
  ArmServo();

  dist_final = 1.25;
  time_final = 1;
  command = 'f';
  MotorOn();
  command = 'b';
  MotorOn();
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
      MotorOn();
      break;

    case 'b':
      MotorOn();
      break;

    case 'r':
      Turn();
      break;

    case 'l':
      Turn();
      break;

    case 'o':
      Serial.println("HERE");
      Arc();
      break;

    case 'a':
      arm_angle_final = 25;
      ArmServo();
      arm_angle_final = 93;
      // ArmServo();
      break;

    case 's':
      shov_t_final = 2;
      shov_angle_final = 180;
      ShovelServo();
      shov_angle_final = 93;
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
      ColorSense();
      break;

      case 'g':
        Begin();
        break;
  }
  command = 0;
}
