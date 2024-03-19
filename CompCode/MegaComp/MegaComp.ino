///////////////////////////////
///-- LIBRARIES & OBJECTS --///
///////////////////////////////
// Include Libraries
#include "DualTB9051FTGMotorShieldUnoMega.h"
#include <Encoder.h>
#include <Servo.h>
#include <QTRSensors.h>

// Create Objects
DualTB9051FTGMotorShieldUnoMega md;
QTRSensors qtr;
Servo arm_servo;
Servo shovel_servo;

//////////////////////////////
///------ STRUCTURES ------///
//////////////////////////////
struct block {
  char face;
  char pos;
  char elev;
  char color;
  bool placed;
};

struct linked_list {
  char command;
  struct linked_list *next;
};


//////////////////////////////
///--- PIN DECLARATIONS ---///
//////////////////////////////
// Digital
int encoder1_pinA = 19;
int encoder1_pinB = 18;
int encoder2_pinA = 20;
int encoder2_pinB = 21;
int arm_servo_pin = 22;
int shovel_servo_pin = 23;
int limit_switch_pin = 32;
int blue_pin = 33;
int green_pin = 34;
int red_pin = 35;
const uint8_t ir_pins[] = { 24, 25, 26, 27, 28, 29, 30, 31 };

// Analog
int dist_pin = A8;
int mag_pin = A9;
int photo_trans_pin = A10;


//////////////////////////////
///--- GLOBAL VARIABLES ---///
//////////////////////////////
// General Vars
float t;
float t_old;
float t_start;
float delta_T;
float pi = 3.1415927;

// Block Info Vars
char rc;
int flag = 33;
int num_blocks;
char block_color;
struct block current_block;
struct block read_block;
struct block red1[10] = {
  { 'w', '4', 'l', false },
  { 'w', '5', 'l', false },
  { 'w', '6', 'l', false },
  { 'e', '4', 'l', false },
  { 'e', '5', 'l', false },
  { 'e', '6', 'l', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false }
};
struct block red2[10] = {
  { 'n', '1', 'l', false },
  { 'n', '2', 'l', false },
  { 'n', '3', 'l', false },
  { 's', '1', 'l', false },
  { 's', '2', 'l', false },
  { 's', '3', 'l', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false }
};
struct block yellow1[10] = {
  { 'n', '1', 'l', false },
  { 'n', '2', 'l', false },
  { 'n', '3', 'l', false },
  { 's', '1', 'l', false },
  { 's', '2', 'l', false },
  { 's', '3', 'l', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false }
};
struct block yellow2[10] = {
  { 'w', '4', 'l', false },
  { 'w', '5', 'l', false },
  { 'w', '6', 'l', false },
  { 'e', '4', 'l', false },
  { 'e', '5', 'l', false },
  { 'e', '6', 'l', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false }
};
struct block blue[8] = {
  { 'n', '1', 'u', false },
  { 'n', '3', 'u', false },
  { 's', '1', 'u', false },
  { 's', '3', 'u', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false }
};

// Trajectory Vars
float KP = 300;
int counts_per_rev = 64;
int gear_ratio = 131;
float wheel_radius = 3.5;   // cm 
float wheel_dist = 19.7;    // cm
float dist_final;           // cm
float turn_angle_final;     // rad
float arc_radius;           // cm
float arc_angle_final;      // rad
float time_final;           // s
char turn_dir;
char travel_dir;
float theta1;
float theta2;
float theta1_final;
float theta2_final;
float theta1_des;
float theta2_des;
float omega1_des;
float omega2_des;
long counts1;
long counts2;
int m1s;
int m2s;

// Travel Variables
float guide1 = 53.34;   // cm
float guide3 = 68.58;   // cm
float guide4 = 0;       // cm, measure dist later
float guide6 = 0;       // cm, measure dist later
int turn_speed = 200;
int straight_speed = 300;
int line_follow_speed = 200;

// Encoder Vars
int encoder1_count;
int encoder2_count;
Encoder encoder1(encoder1_pinA,encoder1_pinB);
Encoder encoder2(encoder2_pinA,encoder2_pinB);

// Arm Servo Vars
int servo_home = 93;
float arm_angle_des;
int arm_angle_start;
int arm_angle_final;
float arm_t_final = 1;
float arm_tol = 0.1;

// Shovel Servo Vars
float shov_angle_des;
int shov_angle_start;
int shov_angle_final;
float shov_t_final = 1;
float shov_tol = 0.1;

// Color Sensor Vars
int color_delay_time = 10;
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
float red_std[3] = { 0, 0, 0 };
float green_std[3] = { 0, 0, 0 };
float blue_std[3] = { 0, 0, 0 };

// Color Calibration Vars
const int color_samples = 100;
int red_calibration_vals[color_samples];
int green_calibration_vals[color_samples];
int blue_calibration_vals[color_samples];
int color_vals[color_samples][3];
int color_ranges[3][3][2] = {
  {{218, 291}, {224, 305}, {-3, 33}},
  {{219, 290}, {228, 299}, {-3, 33}},
  {{218, 291}, {227, 302}, {-2, 32}}
};  // Rows: ranges for each block (ryb)   Cols: Ranges for each LED (rgb)

// Line Following Vars
int kp = 100;
int ir_bias[] = { 144, 93, 93, 93, 136, 93, 98, 119 };
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count];
int ir_unbiased[ir_sensor_count];
float ir_sensor_spacing[] = { 0, 0.8, 1.6, 2.4, 3.2, 4.0, 4.8, 5.6 };  // cm
float ir_dist_desired = 2.8;                                           // cm
float ir_dist_actual;
float ir_error;
float num = 0;
float den = 0;

// Range Finder Vars
float dist_desired = 10;      // cm
float dist_actual = 1000;     // cm
float dist_val;
float a = exp(7.453976699);
float b = -0.907499336;
bool stop = false;

// Mag Vars
float mag_val;
int mag_thresh = 600;

// State Bools
bool start_sequence_bool = true;
bool collect_block_bool = false;
bool travel_bool = false;
bool dump_block_bool = false;
bool color_sense_bool = false;

// Function Bools
bool line_follow_bool = true;
bool dist_sense_bool = false;
bool arm_servo_bool = false;
bool shovel_servo_bool = false;
bool straight_line_bool = false;
bool turn_bool = false;

// Event Bools
bool final_stage = false;
bool color_detected = false;


///////////////////////////////
///--------- SETUP ---------///
///////////////////////////////
void setup() {
  // Begin Serial Comms
  Serial.begin(9600);
  Serial2.begin(9600);
  while (!Serial && !Serial2) {
    ;  // Wait until connected
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
  pinMode(blue_pin, OUTPUT);
  digitalWrite(blue_pin, HIGH);
  pinMode(red_pin, OUTPUT);
  digitalWrite(red_pin, HIGH);
  pinMode(green_pin, OUTPUT);
  digitalWrite(green_pin, HIGH);

  // Check for Start Command and Read Block Info
  Serial.println("Ready For Signal...");
  while (true) {
    if (Serial2.available()) {
      rc = Serial2.read();
      if (rc == flag) {
        ReadBlockInfo();
        break;
      }
    }
  }
}


//////////////////////////////
///--------- LOOP ---------///
//////////////////////////////
void loop() {
  // Line Follow Until a Distance
  if(start_sequence_bool && dist_actual > dist_desired) {
    LineFollow();
    DistSense();
  }
  // Get Ready for Collecting a Block
  else if(start_sequence_bool&& dist_actual <= dist_desired) {
    start_sequence_bool = false;
    collect_block_bool = true;

    // Set Travel State Initial Variables
    t_start = millis();
    t_old = 0;
    dist_final = 1.25;
    time_final = 1;
    counts1 = 0;
    counts2 = 0;
    theta1_des = 0;
    theta2_des = 0;
    encoder1.write(0);
    encoder2.write(0);
    final_stage = false;
    // Initialize Straight Line Variables
    theta1_final = dist_final/wheel_radius;
    theta2_final = dist_final/wheel_radius;
    omega1_des = theta1_final/time_final;
    omega2_des = theta2_final/time_final;
    // Set Servo State Initial Variables
    arm_t_final = 1;
    shov_t_final = 1;
    arm_angle_final = 73;
    shov_angle_final = servo_home;
    arm_angle_start = arm_servo.read();
    shov_angle_start = shovel_servo.read();
  }


  // Collect Block
  if(collect_block_bool){
    CollectBlock();   // Set turn_bool true?
    current_block = {'w', '4', 'l', false};
  }


  // Sense Block Color
  if(color_sense_bool){
    t = (millis()-t_start)/1000;
    ColorSense();
    // Serial.println(current_block.color);
    if(current_block.color != '\0'){
      color_sense_bool = false;
      travel_bool = true;
    }
    else if(t > 10){
      color_sense_bool = false;
      collect_block_bool = true;

      // Set Travel State Initial Variables
      t_start = millis();
      t_old = 0;
      dist_final = 1.25;
      time_final = 1;
      counts1 = 0;
      counts2 = 0;
      theta1_des = 0;
      theta2_des = 0;
      encoder1.write(0);
      encoder2.write(0);
      final_stage = false;
      // Initialize Straight Line Variables
      theta1_final = dist_final/wheel_radius;
      theta2_final = dist_final/wheel_radius;
      omega1_des = theta1_final/time_final;
      omega2_des = theta2_final/time_final;
      // Set Servo State Initial Variables
      arm_t_final = 1;
      shov_t_final = 1;
      arm_angle_final = 73;
      shov_angle_final = servo_home;
      arm_angle_start = arm_servo.read();
      shov_angle_start = shovel_servo.read();
    }
  }


  // Travel to Preffered Block Location
  if(travel_bool){
    Serial.println("Traveling to Loc");
    TravelToLoc();
  }


  // Drop Block Off at Chassis
  if(dump_block_bool){
    Serial.println("Dumping Block");
    DumpBlock();
    dump_block_bool = false;
    // Set travel to dispense bool to true
  }
  

}
