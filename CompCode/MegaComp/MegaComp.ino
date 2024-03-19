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
float speed1;
float speed2;

// Travel Variables
int speed;
float guide1 = 53.34;   // cm
float guide3 = 68.58;   // cm
float guide4 = 0;       // cm, measure dist later
float guide6 = 0;       // cm, measure dist later


// Encoder Vars
int encoder1_count;
int encoder2_count;
Encoder encoder1(encoder1_pinA,encoder1_pinB);
Encoder encoder2(encoder2_pinA,encoder2_pinB);

// Arm Servo Vars
int arm_home = 93;
float arm_angle_des;
int arm_angle_start;
int arm_angle_final;
float arm_t_final = 1;
float arm_tol = 0.1;

// Shovel Servo Vars
int shov_home = 93;
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
int color_ranges[3][3][2];


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
int m1s;
int m2s;

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

// Function Bools
bool line_follow_bool = true;
bool dist_sense_bool = false;
bool arm_servo_bool = false;
bool shovel_servo_bool = false;
bool straight_line_bool = false;
bool turn_bool = false;


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

  // Start Beginning Sequence
}


//////////////////////////////
///--------- LOOP ---------///
//////////////////////////////
void loop() {
  if(start_sequence_bool && dist_actual > dist_desired) {
    Serial.println("Beginning Line Following");
    LineFollow();
    DistSense();
  } 
  else if(start_sequence_bool&& dist_actual <= dist_desired) {
    start_sequence_bool = false;
    collect_block_bool = true;
  }

  if(collect_block_bool){
    Serial.println("Collecting Block");
    CollectBlock();   // Set turn_bool true?
    current_block = {'w', '4', 'l', false};
    collect_block_bool = false;
    travel_bool = true;
  }

  if(travel_bool){
    Serial.println("Travelling to Loc");
    TravelToLoc();
  }

  if(dump_block_bool){
    Serial.println("Dumping Block");
    DumpBlock();
    dump_block_bool = false;
    // Set travel to dispense bool to true
  }
  
}
