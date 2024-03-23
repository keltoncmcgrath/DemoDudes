///////////////////////////////
///-- LIBRARIES & OBJECTS --///
///////////////////////////////
// Include Libraries
#include <DualTB9051FTGMotorShieldUnoMega.h>
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
  char color;
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
char state = 'a';

// Block Info Vars
char rc;
int flag = 33;
int num_blocks;
char block_color;
bool use_first = true;
struct block current_block = { 'e', '5', 'u', false, 'y' };
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
struct block blue[12] = {
  { 'n', '1', 'u', false },
  { 'n', '3', 'u', false },
  { 's', '1', 'u', false },
  { 's', '3', 'u', false },
  { 'n', '2', 'u', false },
  { 'w', '5', 'u', false },
  { 's', '2', 'u', false },
  { 'e', '5', 'u', false },
  { 'w', '4', 'u', false },
  { 'w', '6', 'u', false },
  { 'e', '4', 'u', false },
  { 'e', '6', 'u', false }
};

// Trajectory Vars
float KP = 500;
int counts_per_rev = 64;
int gear_ratio = 131;
float wheel_radius = 3.5;      // cm
float wheel_dist_arc = 20;     // cm
float wheel_dist_turn = 20.8;  // cm
float turn_time = 2;           // s
float dist_final;              // cm
float turn_angle_final;        // rad
float arc_radius;              // cm
float arc_angle_final;         // rad
float time_final;
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
float dist_traveled;
int m1s;
int m2s;

// Travel Variables
float east_guide = 122.5;  // cm
float guide1 = 55;         // cm
float guide2 = 61.5;       // cm
float guide3 = 70;         // cm
float guide4 = 27.84;      // cm, measure dist later
float guide5 = 35.46;      // cm, measure dist later
float guide6 = 43.04;      // cm, measure dist later
int line_follow_speed = 300;

// Encoder Vars
int encoder1_count;
int encoder2_count;
Encoder encoder1(encoder1_pinA, encoder1_pinB);
Encoder encoder2(encoder2_pinA, encoder2_pinB);

// Arm Servo Vars
int servo_home = 93;
int arm_max_angle = 25;
int arm_collect_angle = 73;
int arm_low_dump_angle = 75;
float arm_angle_des;
int arm_angle_start;
int arm_angle_final;
float arm_t_final = 1;
float arm_tol = 0.1;

// Shovel Servo Vars
int shov_max_angle = 150;
int shov_dump_angle = 180;
float shov_angle_des;
int shov_angle_start;
int shov_angle_final;
float shov_t_final = 1;
float shov_tol = 0.1;

// Color Sensor Vars
int color_delay_time = 10;
int block_wait_time = 5;
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
  { { 218, 291 }, { 224, 305 }, { -3, 33 } },
  { { 219, 290 }, { 228, 299 }, { -3, 33 } },
  { { 218, 291 }, { 227, 302 }, { -2, 32 } }
};  // Rows: ranges for each block (ryb)   Cols: Ranges for each LED (rgb)

// Line Following Vars
int kp = 300;
int ir_bias[] = { 144, 93, 93, 93, 136, 93, 98, 119 };
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count];
int ir_unbiased[ir_sensor_count];
float ir_sensor_spacing[] = { 0, 0.8, 1.6, 2.4, 3.2, 4.0, 4.8, 5.6 };  // cm
float ir_dist_desired = 2.8;                                           // cm
float ir_dist_actual;
float ir_error;
bool all_black;
float num = 0;
float den = 0;

// Range Finder Vars
float dump_dist_upper = 11;   // cm
float dump_dist_lower = 11;   // cm
float dist_collect = 14.5;   // cm
float dist_actual = 1000;     // cm
int num_dist_vals = 10;
float dist_val = 0;
float dist_desired;
float a = exp(7.453976699);
float b = -0.907499336;
bool stop = false;

// Mag Vars
float mag_val;
int mag_thresh = 600;

// Event Bools
bool final_stage;
bool final_final_stage;
bool turn_bool;
bool straight_bool;
bool servo_bool;
bool line_follow_bool;


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
  switch (state) {
    // Line follow to dispenser
    case 'a':
      Serial.println("Line Following");
      LineFollow();
      DistSense();
      if (dist_actual <= dist_collect) {
        state = 'b';
        // Set Travel State Initial Variables
        dist_final = 6;
        time_final = 1;
        final_stage = false;
        theta1_final = dist_final / wheel_radius;
        theta2_final = dist_final / wheel_radius;
        ResetTravelVars();
        // Set Servo State Initial Variables
        arm_t_final = 1;
        shov_t_final = 1;
        arm_angle_final = arm_collect_angle;
        shov_angle_final = servo_home;
        arm_angle_start = arm_servo.read();
        shov_angle_start = shovel_servo.read();
      }
      break;


    // Catch a block
    case 'b':
      Serial.println("Collecting Block");
      CollectBlock();
      break;


    // Sense Color of Block
    case 'c':
      Serial.println("Sensing Color");
      t = (millis() - t_start) / 1000;
      // ColorSense();
      // Continue if color is detected
      if (current_block.color != '\0') {
        // DetermineBlockLoc();
        if (current_block.face == 'n' || current_block.face == 'e') {
          arc_angle_final = pi/2;
          arc_radius = 0.8;
          theta1_final = arc_angle_final * (arc_radius+wheel_dist_arc) / wheel_radius;
          theta2_final = arc_angle_final * arc_radius / wheel_radius;
          time_final = 2;
        } 
        else {
          turn_angle_final = pi;
          theta1_final = turn_angle_final * (wheel_dist_turn/2) / wheel_radius;
          theta2_final = -turn_angle_final * (wheel_dist_turn/2) / wheel_radius;
          time_final = 4;
        }
        // Turn Variables
        final_stage = false;
        ResetTravelVars();
        // Change state
        turn_bool = true;
        state = 'd';
      }
      // Else collect another block
      else if (t > block_wait_time) {
        state = 'b';
        // Set Travel State Initial Variables
        dist_final = 4;
        time_final = 1;
        theta1_final = dist_final / wheel_radius;
        theta2_final = dist_final / wheel_radius;
        final_stage = false;
        final_final_stage = false;
        ResetTravelVars();
        // Set Servo State Initial Variables
        arm_t_final = 1;
        shov_t_final = 1;
        arm_angle_final = 73;
        shov_angle_final = servo_home;
        arm_angle_start = arm_servo.read();
        shov_angle_start = shovel_servo.read();
      }
      break;


    // Travel to desired block location
    case 'd':
      // Serial.println("Traveling to Location");
      TravelToLoc();
      break;


    // Dump block onto chassis
    case 'e':
      // Serial.println("Dumping Block");
      DumpBlock();
      break;


    // Return to Dispenser
    case 'f':
      // Serial.println("Returning to Dispenser");
      Return();
      break;
  }
}
