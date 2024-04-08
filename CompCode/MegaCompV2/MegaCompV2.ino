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
  void Reset(void){
    face = '\0';
    pos = '\0';
    elev = '\0';
    placed = false;
    color = '\0';
  }
};

struct node {
  char action[2];
  float final_val[2];
  float duration[2];
  float radius;
  struct node *next;
};

class linked_list {
public:
  struct node *head;
  // Initialization function
  linked_list() {
    this->head = nullptr;
  }
  // New node function
  void AddNode(char action1, float final1, float duration1 = 0, float radius1 = 0, char action2 = '\0', float final2 = 0, float duration2 = 0) {
    struct node *temp = new node;
    temp->action[0] = action1;
    temp->action[1] = action2;
    temp->final_val[0] = final1;
    temp->final_val[1] = final2;
    temp->duration[0] = duration1;
    temp->duration[1] = duration2;
    temp->radius = radius1;
    temp->next = head;
    head = temp;
  }
  // Delete node function
  void DeleteNode() {
    struct node *temp = new node;
    temp = head;
    head = head->next;
    delete temp;
  }
  // Add new node at end of LL
  void AddTailNode(char action1, float final1, float duration1 = 0, float radius1 = 0, char action2 = '\0', float final2 = 0, float duration2 = 0) {
    // Construct node to be added
    struct node *temp = new node;
    temp->action[0] = action1;
    temp->action[1] = action2;
    temp->final_val[0] = final1;
    temp->final_val[1] = final2;
    temp->duration[0] = duration1;
    temp->duration[1] = duration2;
    temp->radius = radius1;
    temp->next = nullptr;
    // Insert as first node if head is null
    if (head == nullptr) {
      temp->next = head;
      head = temp;
      return;
    }
    // Find last node
    struct node *last = head;
    last->next = head->next;
    while (last->next != nullptr) {
      last = last->next;
    }
    // Insert new node
    last->next = temp;
  }
  // Delete node at end of LL
  void DeleteTailNode() {
    struct node *temp = head;
    // Delete current node if it's the last
    if (head->next == nullptr) {
      head = head->next;
      delete temp;
      return;
    }
    // Find second to last node
    while (temp->next->next != nullptr) {
      temp = temp->next;
    }
    // Delete last node and point second to last to null
    delete temp->next;
    temp->next = nullptr;
  }
} directions;


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
int right_turn_pin;
int left_turn_pin;

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
char last_state;

// Block Info Vars
char rc;
int flag = 33;
int num_blocks;
char block_color;
bool use_first = true;
struct block current_block = { 'e', '4', 'u', false, 'y' };
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

// Control Vars
float KP = 95.7;
float KI = 900;
float KD = 1.78;
// float KD = 5.5;
float error1, d_error1, integral_error1;
float error2, d_error2, integral_error2;
// int straight_kp = 125;
int m1s, m2s;
float V1, V2;

// Travel Vars
float dist_traveled;
float dist_final;
float turn_angle_final;        // rad
float arc_radius;              // cm
float arc_angle_final;         // rad
float time_final;
float theta1, theta1_des, theta1_desf, theta1_final, theta1_old;
float theta2, theta2_des, theta2_desf, theta2_final, theta2_old;
float omega1, omega1_des;
float omega2, omega2_des;
long counts1;
long counts2;
float theta_alpha = 0.1;

// Travel Constants
int counts_per_rev = 64;
int gear_ratio = 131;
float wheel_radius = 3.5;    // cm
float wheel_dist = 19;       // cm
float turn_time = 1.5;       // s
float arc_time_big = 3;      // s
float arc_time_little = 2.5;
float ir_to_wheel = 6;

// Travel Variables
float east_guide = 122.5;   // cm
float south_guide = 68.58;  // cm
float north_guide = 34.5;   // cm
float guide1 = 55;          // cm
float guide2 = 59.5;        // cm
float guide3 = 66;          // cm
float guide4 = 52;          // cm, measure dist later
float guide5 = 61;          // cm, measure dist later
float guide6 = 68;          // cm, measure dist later
float collect_dist = 5.5;   // cm
int line_follow_speed = 300;
int line_speed;

// Encoder Vars
int encoder1_count;
int encoder2_count;
Encoder encoder1(encoder1_pinA, encoder1_pinB);
Encoder encoder2(encoder2_pinA, encoder2_pinB);

// Arm Servo Vars
int servo_home = 93;
int arm_max_angle = 20;
int arm_collect_angle = 90;
int arm_low_dump_angle = 70;
float arm_angle_des;
int arm_angle_start;
int arm_angle_final;
float arm_t_final = 1;
float arm_tol = 0.1;

// Shovel Servo Vars
int shov_max_angle = 20;
int shov_low_dump_angle = 73;
int shov_dump_angle = 0;
int shov_collect_angle = 120;
float shov_angle_des;
int shov_angle_start;
int shov_angle_final;
float shov_t_final = 1;
float shov_tol = 0.1;

// Color Sensor Vars
int color_delay_time = 10;
int block_wait_time = 5;
bool is_color = false;
long red_sum = 0;
long green_sum = 0;
long blue_sum = 0;
long red_val;
long green_val;
long blue_val;
float red_avg[3];
float green_avg[3];
float blue_avg[3];
float red_std[3] = { 0, 0, 0 };
float green_std[3] = { 0, 0, 0 };
float blue_std[3] = { 0, 0, 0 };

// Color Calibration Vars
const int color_samples = 10;
int red_calibration_vals[color_samples];
int green_calibration_vals[color_samples];
int blue_calibration_vals[color_samples];
int color_vals[color_samples][3];
int color_ranges[3][3][2] = {
  { { 614, 687 },   { 36, 95 },    { 4, 53 } },
  { { 837, 910 }, { 627, 700 },  { 47, 108 } },
  {   { 25, 82 },  { 71, 134 }, { 113, 180 } }
};  // Rows: ranges for each block (ryb)   Cols: Ranges for each LED (rgb)

// Line Following Vars
int kp = 250; // 200
int ki = 500;
int kd = 4;
float ir_error, ir_error_last, ir_d_error, ir_integral_error;
int ir_bias[] = { 140, 94, 128, 134, 140, 140, 140, 140 };
const uint8_t ir_sensor_count = 8;
uint16_t ir_values[ir_sensor_count];
int ir_unbiased[ir_sensor_count];
float ir_sensor_spacing[] = { 0, 0.8, 1.6, 2.4, 3.2, 4.0, 4.8, 5.6 };  // cm
float ir_dist_desired = 2.4; //2.8                                     // cm
float ir_dist_actual;
bool all_black;
float num = 0;
float den = 0;

// Range Finder Vars
float dist_alpha = 0.01;
float dump_dist_upper = 15.5;  // cm
float dump_dist_lower = 10.9;  // cm
float dist_collect = 12;       // cm
float dist_actual = 1000;      // cm
float dist_to_wall = 11;
int num_dist_vals = 100;
float dist_val = 0;
float last_dist_val = 0;
float dist_desired;
float a = exp(7.453976699);
float b = -0.907499336;
bool stop = false;

// Mag Vars
float mag_alpha = 0.05;
int mag_thresh = 0;
int mag_val;
int mag_val_last;
int mag_ss;

// Event Bools
bool final_stage;
bool final_final_stage;
bool turn_bool;
bool straight_bool;
bool servo_bool;
bool line_follow_bool;
bool new_action;
bool next_node;
bool line_dist;
bool second_line;
bool last_line_state;
bool ramp_down;
bool home_dispense = true;


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
  shovel_servo.write(shov_max_angle);

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

  // Setup turn LEDs
  pinMode(right_turn_pin, OUTPUT);
  pinMode(left_turn_pin, OUTPUT);

  // Check for Start Command and Read Block Info
  Serial.println("Ready For Signal...");
  while (true) {
    if (Serial2.available()) {
      rc = Serial2.read();
      if (rc == flag) {
        shovel_servo.write(servo_home);
        ReadBlockInfo();
        break;
      }
    }
  }
  delay(300);
}


//////////////////////////////
///--------- LOOP ---------///
//////////////////////////////
void loop() {
  switch (state) {
    // Initiate Dispenser Travel
    case 'a':
      HallEffect();
      mag_ss = mag_val;
      current_block.Reset();
      directions.AddTailNode('l', dist_collect, 0, 0, 'a', arm_collect_angle, 1);
      line_dist = true;
      new_action = true;
      last_state = state;
      state = 'd';
      break;


    // Collect Block
    case 'b':
      CollectBlock();
      new_action = true;
      last_state = state;
      state = 'd';
      break;


    // Color Sense
    case 'c':
      t = (millis() - t_start) / 1000;
      // Detect if Ramp is Down
      HallEffect();
      if(mag_val >= mag_ss + mag_thresh || mag_val <= mag_ss - mag_thresh){
        ramp_down = true;
      } else if(!ramp_down) {
        state = 'b';
      }
      // Sense Color and change state
      if(current_block.color == '\0'){
        ColorSense();
      } else if (ramp_down) {  //current_block.color != '\0'
        DetermineBlockLoc();
        Serial.print(current_block.face);
        Serial.print('\t');
        Serial.print(current_block.pos);
        Serial.print('\t');
        Serial.print(current_block.elev);
        Serial.print('\t');
        Serial.println(current_block.color);
        GetDirections();
        ramp_down = false;
        new_action = true;
        last_state = state;
        state = 'd';
      }
      // Else collect another block
      if (t > block_wait_time) {
        state = 'b';
      }
      break;


    // Travel
    case 'd':
      Travel();
      break;


    // Dump Block
    case 'e':
      DumpBlock();
      new_action = true;
      last_state = state;
      state = 'd';
      break;


    // Get Directions Home
    case 'f':
      GetDirectionsHome();
      new_action = true;
      last_state = state;
      state = 'd';
      break;
  }
}